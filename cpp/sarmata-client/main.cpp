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

#include "libsarmata-client/asr_service.grpc.pb.h"
#include "libsarmata-client/RemoteSession.h"
#include "libsarmata-client/ASRSessionUtils.h"

std::vector<short> loadWave(const std::string & path);

std::string fileContent(const std::string & path);

ASRSessionSettings loadSettings(const std::string & path);

int main(int ac, char* av[])
{
    using grpc::Channel;
    using grpc::ClientContext;
    using grpc::Status;
    using grpc::ClientReaderWriter;
    using namespace sarmata;

    if (ac < 4)
    {
        std::cout << "Usage:\n" << 
        av[0] << " host-address audio.wav grammar.txt [options.txt]" << std::endl;
        return 1;
    }
    auto data = loadWave(av[2]);
    auto grammar = fileContent(av[3]);
    ASRSessionSettings settings;
    if (ac == 5)
    {
        settings = loadSettings(av[4]);
    }
    settings["grammar_data"] = grammar;
    
    RemoteSession session(av[1]);
    session.Open("", settings);
    session.AddSamples(data);
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
            auto showPath = [](const Phrase & path)
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

std::vector<short> loadWave(const std::string & path)
{
    struct WAV_HEADER
    {
        char               RIFF[4];        // RIFF Header      Magic header
        unsigned int       ChunkSize;      // RIFF Chunk Size  
        char               WAVE[4];        // WAVE Header      
        char               fmt[4];         // FMT header       
        unsigned int       Subchunk1Size;  // Size of the fmt chunk                                
        unsigned short     AudioFormat;    // Audio format 1=PCM,6=mulaw,7=alaw, 257=IBM Mu-Law, 258=IBM A-Law, 259=ADPCM 
        unsigned short     NumOfChan;      // Number of channels 1=Mono 2=Sterio                   
        unsigned int       SamplesPerSec;  // Sampling Frequency in Hz                             
        unsigned int       bytesPerSec;    // bytes per second 
        unsigned short     blockAlign;     // 2=16-bit mono, 4=16-bit stereo 
        unsigned short     bitsPerSample;  // Number of bits per sample      
        char               Subchunk2ID[4]; // "data"  string   
        unsigned int       Subchunk2Size;  // Sampled data length    
    } header;
    std::ifstream file(path, std::ios::binary);
    file.read((char*)&header, sizeof(header));
    std::vector<short> data(header.Subchunk2Size/sizeof(short));
    file.read((char*)data.data(), header.Subchunk2Size);
    return data;
}

std::string fileContent(const std::string & path)
{
    std::ifstream t(path);
    std::stringstream buffer;
    buffer << t.rdbuf();
    return buffer.str();
}

ASRSessionSettings loadSettings(const std::string & path)
{
    ASRSessionSettings settings;
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



