#include "RemoteSession.h"

#include <cstring>

namespace sarmata
{

RemoteSession::RemoteSession(const std::string & host, const std::string & token, const ASRSessionSettings & settings)
    : stub_(ASR::NewStub(
        grpc::CreateChannel("localhost:50051", grpc::InsecureChannelCredentials())))
    , stream_(stub_->Recognize(&context_))
{

    InitialRecognizeRequest initial;
    for (const auto & field : settings)
    {
        auto * configField = initial.add_config();
        configField->set_key(field.first);
        configField->set_value(field.second);
    }
    RecognizeRequest request;
    *request.mutable_initial_request() = initial;
    stream_->Write(request);
}

void RemoteSession::AddSamples(const std::vector<short> & data)
{
    AudioRequest audio;
    std::string content(data.size() * sizeof(short), 0);
    std::memcpy( (char*)content.data(), data.data(), content.size());
    audio.set_content(content);
    audio.set_end_of_stream(false);
    
    RecognizeRequest request;
    *request.mutable_audio_request() = audio;
    stream_->Write(request);
}

void RemoteSession::EndOfStream()
{
    AudioRequest audio;
    audio.set_end_of_stream(true);
    RecognizeRequest request;
    *request.mutable_audio_request() = audio;
    stream_->Write(request);

}

RecognizeResponse RemoteSession::WaitForResponse(void)
{
    RecognizeResponse response;
    stream_->Read(&response);
    return response;
}

}
