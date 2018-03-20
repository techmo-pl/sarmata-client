#include <iostream>
#include <fstream>
#include <sstream>

#include <boost/program_options.hpp>
#include <google/protobuf/text_format.h>

#include "sarmata_client.h"
#include "wave-utils.h"
#include "VERSION.h"


namespace po = boost::program_options;

bool FileExists(const std::string & path) {
    if (FILE * file = fopen(path.c_str(), "r")) {
        fclose(file);
        return true;
    }
    else {
        return false;
    }
}

std::string FileContent(const std::string & path) {
    std::ifstream t(path);
    std::stringstream buffer;
    buffer << t.rdbuf();
    return buffer.str();
}

std::string ProtobufMessageToString(const google::protobuf::Message & message) {
    grpc::string out_str;
    google::protobuf::TextFormat::PrintToString(message, &out_str);
    return out_str;
}

std::map<std::string, std::string> ReadServiceSettingsOption(const std::string & settings_string) {
    std::map<std::string, std::string> service_settings;
    // split by ';'
    std::vector<std::string> settings_lines;
    //boost::split(settings_lines, settings_string, boost::is_any_of(";"));
    std::stringstream settings_stream(settings_string);
    std::string setting_line;
    while (std::getline(settings_stream, setting_line, ';')) {
        settings_lines.push_back(setting_line);
    }

    std::cout << "Passing session settings:" << std::endl;
    for (const auto & line : settings_lines) {
        // split by '='
        std::vector<std::string> key_value;
        //boost::split(key_value, line, boost::is_any_of("="));
        std::stringstream key_value_steam(line);
        std::string field;
        while (std::getline(key_value_steam, field, '=')) {
            key_value.push_back(field);
        }

        if (key_value.size() == 2) {
            const auto key = key_value[0];
            service_settings[key] = key_value[1];
            std::cout << "key: " << key
                    << " | value: " << service_settings[key] << std::endl;
        }
        else {
            std::cout << "Skipping invalid session settings line: " << line << std::endl;
        }
    }

    return service_settings;
}

techmo::sarmata::SarmataSessionConfig CreateSarmataSessionConfig(const po::variables_map & userOptions) {
    techmo::sarmata::SarmataSessionConfig config;
    config.session_id = userOptions["session-id"].as<std::string>();
    config.service_settings = ReadServiceSettingsOption(userOptions["service-settings"].as<std::string>());
    config.max_alternatives = userOptions["max-alternatives"].as<int>();

    config.grammar_name = userOptions.count("grammar-name") ? userOptions["grammar-name"].as<std::string>() : "";
    config.grammar_data = [&]() {
        const auto grammarPath = userOptions.count("grammar") ? userOptions["grammar"].as<std::string>() : "";
        if (not grammarPath.empty() and not FileExists(grammarPath)) {
            std::cerr << "Grammar file does not exist: " << grammarPath << std::endl;
            return std::string{};
        }
        return grammarPath.empty() ? std::string{} : FileContent(grammarPath);
    }();

    if (userOptions.count("no-match-threshold")) { config.no_match_threshold = userOptions["no-match-threshold"].as<double>(); }
    if (userOptions.count("no-input-timeout")) { config.no_input_timeout = userOptions["no-input-timeout"].as<int>(); }
    if (userOptions.count("recognition-timeout")) { config.recognition_timeout = userOptions["recognition-timeout"].as<int>(); }
    if (userOptions.count("speech-complete-timeout")) { config.speech_complete_timeout = userOptions["speech-complete-timeout"].as<int>(); }
    if (userOptions.count("speech-incomplete-timeout")) { config.speech_incomplete_timeout = userOptions["speech-incomplete-timeout"].as<int>(); }

    return config;
}

int DefineGrammar(const techmo::sarmata::SarmataSessionConfig & config, const techmo::sarmata::SarmataClient & sarmata_client) {
    if (config.grammar_name.empty()) {//bad usage
        std::cerr << "Option --grammar-name is required when running with --define-grammar." << std::endl;
        return 1;
    }

    std::cout << (config.grammar_data.empty()
                ? "Deleting grammar "
                : "Defining grammar ")
            << config.grammar_name << std::endl;

    const auto response = sarmata_client.DefineGrammar(config);

    std::cout << ProtobufMessageToString(response) << std::endl;
    std::cout << "DefineGrammar returned status " << response.status() << " " << response.error() << std::endl;

    return not response.ok();
}

int Recognize(const po::variables_map & userOptions, techmo::sarmata::SarmataSessionConfig & config, const techmo::sarmata::SarmataClient & sarmata_client) {
    if (not (userOptions.count("grammar") || userOptions.count("grammar-name"))) {//bad usage
        std::cerr << "Options --grammar-name or --grammar are required when running with --wave-path." << std::endl;
        return 1;
    }

    const auto wavePath = userOptions["wave-path"].as<std::string>();
    if (not FileExists(wavePath)) {
        std::cerr << "Wave file does not exist: " << wavePath << std::endl;
        return 1;
    }

    const auto wave = ReadWaveFile(userOptions["wave-path"].as<std::string>());
    std::vector<short> waveSamples(wave.audioBytes.size() / sizeof(short), 0);
    std::memcpy((char*)waveSamples.data(), wave.audioBytes.data(), wave.audioBytes.size());

    if (config.grammar_name.empty() && config.grammar_data.empty()) {
        std::cerr << "Neither grammar name nor grammar data specified (both are empty)." << std::endl;
        return 1;
    }

    config.audio_sample_rate_hz = wave.header.samplesPerSec;
    const auto responses = sarmata_client.Recognize(config, wave.audioBytes);

    for (const auto& response : responses) {
        std::cout << ProtobufMessageToString(response) << std::endl;
    }

    return 0;
}

po::options_description CreateOptionsDescription(void) {
    // command line options
    po::options_description optionsDescription("Sarmata ASR gRPC client options:");
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
            ("define-grammar", "If present, will perform DefineGrammar call for given --grammar-name "
             "and --grammar (will not call Recognize).");
    return optionsDescription;
}

int main(int argc, char * argv[]) {
    try {
        po::options_description optionsDescription(CreateOptionsDescription());
        po::variables_map userOptions;
        po::store(po::command_line_parser(argc, argv).options(optionsDescription).run(), userOptions);
        std::cout << "Sarmata ASR gRPC client " << LIBSARMATA_CLIENT_VERSION << std::endl;
        if (userOptions.count("help")) {
            std::cout << optionsDescription;
            return 0;
        }
        po::notify(userOptions);

        techmo::sarmata::SarmataSessionConfig config = CreateSarmataSessionConfig(userOptions);
        techmo::sarmata::SarmataClient sarmata_client{ userOptions["service-address"].as<std::string>() };

        if (userOptions.count("define-grammar")) {
            return DefineGrammar(config, sarmata_client);
        }
        else if (userOptions.count("wave-path")) {
            return Recognize(userOptions, config, sarmata_client);
        }
    }
    catch (const std::exception& e) {
        std::cerr << e.what() << std::endl;
        return 1;
    }

    return 0;
}
