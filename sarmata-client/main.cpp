#include <iostream>
#include <thread>
#include <chrono>

#include <grpc/grpc.h>
#include <grpc++/channel.h>
#include <grpc++/client_context.h>
#include <grpc++/create_channel.h>
#include <grpc++/security/credentials.h>

#include "libsarmata-client/asr_service.grpc.pb.h"

int main()
{
    using grpc::Channel;
    using grpc::ClientContext;
    using grpc::Status;
    using grpc::ClientReaderWriter;
    using namespace sarmata;
    
    auto channel = grpc::CreateChannel("localhost:50051", grpc::InsecureChannelCredentials());
    
    ClientContext context;

    auto stub_ = ASR::NewStub(channel);
    
    std::shared_ptr<ClientReaderWriter<RecognizeRequest, RecognizeResponse> > stream(
        stub_->Recognize(&context));

    std::thread writer([stream]() {
        InitialRecognizeRequest initial;
        initial.set_token("dawid");
        RecognizeRequest request;
        *request.mutable_initial_request() = initial;
        while (true)
        {
            stream->Write(request);
	        std::this_thread::sleep_for(std::chrono::milliseconds(1000));
        }
    });

    RecognizeResponse response;
    while (stream->Read(&response)) {
      std::cout << "Got message " << response.status() << std::endl;
    }
    writer.join();
    Status status = stream->Finish();
    if (!status.ok()) {
      std::cout << "RouteChat rpc failed." << std::endl;
    }
    
}
