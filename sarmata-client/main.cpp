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
    
    RecognizeResponse res;
    do{
        res = session.WaitForResponse();
        std::cout << res.status() << std::endl;
    } while(res.status() != EMPTY && res.status() != END_OF_AUDIO);
    
    //auto channel = grpc::CreateChannel("localhost:50051", grpc::InsecureChannelCredentials());
    //
    //ClientContext context;
    //
    //auto stub_ = ASR::NewStub(channel);
    //
    ////// define gramamr
    ////{
    ////    
    ////    DefineGrammarRequest req;
    ////    req.set_name("client_test");
    ////    DefineGrammarRespone res;     
    ////    Status status = stub_->DefineGrammar(&context, req, &res);
    ////    
    ////    if (!status.ok()) {
    ////        std::cout << "DefineGrammar rpc failed: " << status.error_message() << std::endl;
    ////        return false;
    ////    }
    ////    
    ////    std::cout << res.grammarname() << std::endl;
    ////}
    //
    //std::shared_ptr<ClientReaderWriter<RecognizeRequest, RecognizeResponse> > stream(
    //    stub_->Recognize(&context));
    //
    //std::thread writer([stream]() {
    //    InitialRecognizeRequest initial;
    //    initial.set_token("dawid");
    //    RecognizeRequest request;
    //    *request.mutable_initial_request() = initial;
    //    std::cout << "writing" << std::endl;
    //    while (true)
    //    {
    //        stream->Write(request);
    //        std::cout << "message send" << std::endl;
	//        std::this_thread::sleep_for(std::chrono::milliseconds(1000));
    //        
    //    }
    //});
    //
    //RecognizeResponse response;
    //std::cout << "reading" << std::endl;
    //while (stream->Read(&response)) {
    //  std::cout << "Got message " << response.status() << std::endl;
    //}
    //writer.join();
    //Status status = stream->Finish();
    //if (!status.ok()) {
    //  std::cout << "RouteChat rpc failed." << std::endl;
    //}
    
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
    while (std::getline(file, line))
    {
        //if comment
        if (line.size() && line[0] == '#') continue;
        std::stringstream ss(line);
        std::string key, value;
        ss >> key >> value;
        settings.emplace(key, value);
    }
    return settings;
}



