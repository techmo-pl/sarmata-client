#include <iostream>
#include <thread>
#include <chrono>
#include <fstream>
#include <sstream>

#include <boost/program_options.hpp>
#include <grpc/grpc.h>
#include <grpc++/channel.h>
#include <grpc++/client_context.h>
#include <grpc++/create_channel.h>
#include <grpc++/security/credentials.h>

#include "libsarmata-client/sarmata_asr.grpc.pb.h"
#include "libsarmata-client/RemoteSession.h"
#include "libsarmata-client/ASRSessionUtils.h"
#include "wave-utils.h"
#include "VERSION.h"


namespace po = boost::program_options;

bool fileExists(const std::string & path) {
    if (FILE * file = fopen(path.c_str(), "r")) {
        fclose(file);
        return true;
    }
    else {
        return false;
    }
}

std::string fileContent(const std::string & path) {
    std::ifstream t(path);
    std::stringstream buffer;
    buffer << t.rdbuf();
    return buffer.str();
}

std::map<std::string, std::string> loadSettings(const std::string & path) {
    std::map<std::string, std::string> settings;
    std::ifstream file(path);
    std::string line;
    std::cout << "Additional options: " << std::endl;
    while (std::getline(file, line)) {
        //if comment
        if (line.size() && line[0] == '#') { continue; }
        std::stringstream ss(line);
        std::string key, value;
        ss >> key >> value;
        if (key.size() && value.size()) {
            settings.emplace(key, value);
            std::cout << "\"" << key << "\" : \"" << value << "\"" << std::endl;
        }
    }
    return settings;
}

int defineGrammar(const po::variables_map & userOptions) {
    if (not userOptions.count("grammar-name")) {
        std::cerr << "Option --grammar-name is required when running with --define-grammar." << std::endl;
        return 1;
    }

    const auto grammarPath = userOptions["grammar"].as<std::string>();

    if (not grammarPath.empty() and not fileExists(grammarPath)) {
        std::cerr << "Grammar file does not exist: " << grammarPath << std::endl;
        return 1;
    }

    const auto grammar = grammarPath.empty() ? "" : fileContent(grammarPath);

    techmo::sarmata::RemoteSession session(userOptions["service-address"].as<std::string>());
    const auto response = session.PreDefineGrammar(userOptions["grammar-name"].as<std::string>(), grammar);

    std::cout << "DefineGrammar returned status " << response.status() << " " << response.error() << std::endl;

    return not response.ok();
}

ASRSessionSettings CreateASRSessionSettings(const po::variables_map& userOptions, int sample_rate) {
    ASRSessionSettings settings;

    settings.sessionId = userOptions["session-id"].as<std::string>();

    if (userOptions.count("service-settings")) {
        settings.config = loadSettings(userOptions["service-settings"].as<std::string>());
    }

    settings.sampleRateHertz = sample_rate;
    settings.maxAlternatives = userOptions["max-alternatives"].as<int>();

    settings.grammarName = userOptions.count("grammar-name") ? userOptions["grammar-name"].as<std::string>() : "";
    settings.grammarData = [&]() {
        const auto grammarPath = userOptions.count("grammar") ? userOptions["grammar"].as<std::string>() : "";
        if (not grammarPath.empty() and not fileExists(grammarPath)) {
            std::cerr << "Grammar file does not exist: " << grammarPath << std::endl;
            return std::string{};
        }
        return grammarPath.empty() ? std::string{} : fileContent(grammarPath);
    }();

    if (userOptions.count("no-match-threshold")) { settings.noMatchThreshold = userOptions["no-match-threshold"].as<double>(); }
    if (userOptions.count("no-input-timeout")) { settings.noInputTimeout = userOptions["no-input-timeout"].as<int>(); }
    if (userOptions.count("recognition-timeout")) { settings.recognitionTimeout = userOptions["recognition-timeout"].as<int>(); }
    if (userOptions.count("speech-complete-timeout")) { settings.speechCompleteTimeout = userOptions["speech-complete-timeout"].as<int>(); }
    if (userOptions.count("speech-incomplete-timeout")) { settings.speechIncompleteTimeout = userOptions["speech-incomplete-timeout"].as<int>(); }

    return settings;
}

po::options_description CreateOptionsDescription(void) {
    // command line options
    po::options_description optionsDescription("Dictation ASR gRPC client options:");
    optionsDescription.add_options()
            ("help", "Print help message.")
            ("service-address", po::value<std::string>()->required(),
             "IP address and port (address:port) of a service the client will connect to.")
            ("wave-path", po::value<std::string>(),
             "Path to wave file with audio content to be sent to service via RPC.")
            ("grammar-name", po::value<std::string>(), "Name (ID) of the grammar in the service's grammar cache.")
            ("grammar", po::value<std::string>(), "SRGS grammar file (ABNF or XML format accepted).")
            ("session-id", po::value<std::string>()->default_value(""),
             "Session ID to be passed to the service. If not specified, the service will generate a default session ID itself.")
            ("service-settings", po::value<std::string>()->default_value(""),
             "Semicolon-separated list of key=value pairs defining settings to be sent to service via gRPC request.")
            ("max-alternatives", po::value<int>()->default_value(1),
             "Maximum number of recognition hypotheses to be returned.")
            ("no-match-threshold", po::value<double>(),
             "Confidence acceptance threshold.")
            ("no-input-timeout", po::value<int>(),
             "MRCPv2 No-Input-Timeout in milliseconds.")
            ("recognition-timeout", po::value<int>(),
             "MRCPv2 Recognition-Timeout in milliseconds.")
            ("speech-complete-timeout", po::value<int>(),
             "MRCPv2 Speech-Complete-Timeout in milliseconds.")
            ("speech-incomplete-timeout", po::value<int>(),
             "MRCPv2 Speech-Incomplete-Timeout in milliseconds.")
            ("define-grammar", "If present, will perform DefineGrammar call for given --grammar-name"
             "and --grammar (will not call Recognize).");
    return optionsDescription;
}

int main(int argc, char* argv[]) {
    using grpc::Channel;
    using grpc::ClientContext;
    using grpc::Status;
    using grpc::ClientReaderWriter;
    using namespace techmo::sarmata;

    try {
        po::options_description optionsDescription(CreateOptionsDescription());
        po::variables_map userOptions;
        po::store(po::command_line_parser(argc, argv).options(optionsDescription).run(), userOptions);
        po::notify(userOptions);

        std::cout << "Sarmata ASR gRPC client " << LIBSARMATA_CLIENT_VERSION << std::endl;

        if (userOptions.count("help")) {
            std::cout << optionsDescription;
            return 0;
        }

        if (userOptions.count("define-grammar")) {
            return defineGrammar(userOptions);
        }

        if (userOptions.count("wave-path")) {
            if (not (userOptions.count("grammar") || userOptions.count("grammar-name"))) {//bad usage
                std::cerr << "Usage (Recognize): " << argv[0] << " --service-address host:address --wave-path /path/to/audio.wav --grammar-name name"
                        "\n   or " << argv[0] << " --service-address host:address --wave-path /path/to/audio.wav --grammar data" << std::endl;
                return 1;
            }

            const auto wavePath = userOptions["wave-path"].as<std::string>();
            if (not fileExists(wavePath)) {
                std::cerr << "Wave file does not exist: " << wavePath << std::endl;
                return 1;
            }

            const auto wave = ReadWaveFile(userOptions["wave-path"].as<std::string>());
            std::vector<short> waveSamples(wave.audioBytes.size() / sizeof(short), 0);
            std::memcpy((char*)waveSamples.data(), wave.audioBytes.data(), wave.audioBytes.size());

            const ASRSessionSettings settings = CreateASRSessionSettings(userOptions, wave.header.samplesPerSec);

            if (settings.grammarName.empty() && settings.grammarData.empty()) {
                std::cerr << "Neither grammar name nor grammar data specified (both are empty)." << std::endl;
                return 1;
            }

            RemoteSession session(userOptions["service-address"].as<std::string>());
            session.Open(settings);
            session.AddSamples(waveSamples);
            session.EndOfStream();

            RecognizeResponse response;
            do {
                response = session.WaitForResponse();
                if (response.status() != EMPTY) {
                    std::cout << "event " << response.status() << " at " << response.event_time() << std::endl;
                }
                if (response.results().size()) {
                    auto showPath = [](const RecognizedPhrase & path) {
                        using namespace std;
                        int w = 0, gp = 0, gs = 0;
                        for (const auto & word : path.words()) {
                            cout << ++w << ". " << word.transcript() << " [" << word.start() << " - " << word.end() << "] (" << word.logprob() << ") " << word.confidence() <<  endl;
                        }
                        if (path.semantic_interpretation().size()) {
                            cout << "SI:" << endl;
                            cout << path.semantic_interpretation() << endl;
                        }
                        cout << "Path confidence: " << path.confidence() << endl;
                    };
                    for (int i = 0; i < response.results().size(); i++) {
                        std::cout << "paths nr " << i + 1 << ":" << std::endl;
                        showPath(response.results(i));
                    }
                }
            } while (response.status() != EMPTY && response.status() != END_OF_AUDIO);
        }
    }
    catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
        return 1;
    }

    return 0;
}
