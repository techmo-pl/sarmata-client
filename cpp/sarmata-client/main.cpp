#include <iostream>
#include <thread>
#include <chrono>
#include <fstream>
#include <sstream>

#include <grpc/grpc.h>
#include <grpc++/channel.h>
#include <grpc++/client_context.h>
#include <grpc++/create_channel.h>
#include <grpc++/security/credentials.h>

#include "libsarmata-client/sarmata_asr.grpc.pb.h"
#include "libsarmata-client/RemoteSession.h"
#include "libsarmata-client/ASRSessionUtils.h"
#include "wave-utils.h"

std::vector<short> loadWave(const std::string & path);

std::string fileContent(const std::string & path);

std::map<std::string, std::string> loadSettings(const std::string & path);

int main(int ac, char* av[])
{
    using grpc::Channel;
    using grpc::ClientContext;
    using grpc::Status;
    using grpc::ClientReaderWriter;
    using namespace techmo::sarmata;

    if (ac < 4)
    {
        std::cout << "Usage:\n" << 
        av[0] << " host-address audio.wav grammar.txt [options.txt]" << std::endl;
        return 1;
    }

    RemoteSession session(av[1]);

    auto wave = ReadWaveFile(av[2]);
    std::vector<short> waveSamples(wave.audioBytes.size() / sizeof(short), 0);
    std::memcpy((char*)waveSamples.data(), wave.audioBytes.data(), wave.audioBytes.size());

    auto grammar = fileContent(av[3]);

    ASRSessionSettings settings;
    if (ac == 5)
    {
        settings.config = loadSettings(av[4]);
    }
    settings.sampleRateHertz = wave.header.samplesPerSec;
    constexpr auto grammar_name = "pre-defined-grammar";
    settings.grammarName = grammar_name;

    try
    {
        session.PreDefineGrammar(grammar_name, grammar);
    }
    catch (const std::exception& e)
    {
        std::cout << e.what() << std::endl;
    }

    session.Open("", settings);
    session.AddSamples(waveSamples);
    session.EndOfStream();
    
    RecognizeResponse response;
    do{
        response = session.WaitForResponse();
        if (response.status() != EMPTY)
        {
            std::cout << "event " << response.status() << " at " << response.event_time() << std::endl;
        }
        if (response.results().size())
        {
            auto showPath = [](const RecognizedPhrase & path)
            {
                using namespace std;
                int w = 0, gp = 0, gs = 0;
                for (const auto & word : path.words())
                {
                    cout << ++w << ". " << word.transcript() << " [" << word.start() << " - " << word.end() << "] (" << word.logprob() << ") " << word.confidence() <<  endl;
                }
                if (path.semantic_interpretation().size())
                {
                    cout << "SI:" << endl;
                    cout << path.semantic_interpretation() << endl;
                }
                cout << "Path confidence: " << path.confidence() << endl;
            };
            for (int i = 0; i < response.results().size(); i++)
            {
                std::cout << "paths nr " << i + 1 << ":" << std::endl;
                showPath(response.results(i));
            }
        }
    } while(response.status() != EMPTY && response.status() != END_OF_AUDIO);
    
}

std::string fileContent(const std::string & path)
{
    std::ifstream t(path);
    std::stringstream buffer;
    buffer << t.rdbuf();
    return buffer.str();
}

std::map<std::string, std::string> loadSettings(const std::string & path)
{
    std::map<std::string, std::string> settings;
    std::ifstream file(path);
    std::string line;
    std::cout << "Additional options: " << std::endl;
    while (std::getline(file, line))
    {
        //if comment
        if (line.size() && line[0] == '#') continue;
        std::stringstream ss(line);
        std::string key, value;
        ss >> key >> value;
        if (key.size() && value.size())
        {
            settings.emplace(key, value);
            std::cout << "\"" << key << "\" : \"" << value << "\"" << std::endl;
        }
    }
    return settings;
}
