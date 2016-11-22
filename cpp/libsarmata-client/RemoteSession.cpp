#include "RemoteSession.h"

#include <cstring>

namespace sarmata
{

RemoteSession::RemoteSession(const std::string & host)
    : host_(host)
    , samplesStreamCompleted_(false)
{}

RemoteSession::~RemoteSession()
{
    if (stream_)
    {
		if(!samplesStreamCompleted_)
		{
			stream_->WritesDone();
		}
        stream_->Finish();
    }
}

void RemoteSession::Open(const std::string & token, const ASRSessionSettings & settings)
{
    stub_ = ASR::NewStub(grpc::CreateChannel(host_, grpc::InsecureChannelCredentials()));
    stream_ = stub_->Recognize(&context_);
    samplesStreamCompleted_ = false;
    
    InitialRecognizeRequest initial;
    for (const auto & field : settings)
    {
        auto * configField = initial.add_config();
        configField->set_key(field.first);
        configField->set_value(field.second);
    }
    RecognizeRequest request;
    *request.mutable_initial_request() = initial;
    bool ok = stream_->Write(request);
    if (!ok)
    {
        throw std::runtime_error("Stream closed");  //todo: add own exception hierarchy
    }
}

void RemoteSession::AddSamples(const std::vector<short> & data)
{
    if(samplesStreamCompleted_)
    {
        throw std::runtime_error("Stream closed");
    }
	
    AudioRequest audio;
    std::string content(data.size() * sizeof(short), 0);
    std::memcpy( (char*)content.data(), data.data(), content.size());
    audio.set_content(content);
    audio.set_end_of_stream(false);
    
    RecognizeRequest request;
    *request.mutable_audio_request() = audio;
    bool ok = stream_->Write(request);
    if (!ok)
    {
        throw std::runtime_error("Stream closed");  //todo: add own exception hierarchy
    }
}

void RemoteSession::EndOfStream()
{
    if(samplesStreamCompleted_)
    {
        throw std::runtime_error("Stream closed");
    }
	
    AudioRequest audio;
    audio.set_end_of_stream(true);
    RecognizeRequest request;
    *request.mutable_audio_request() = audio;
    bool ok = stream_->Write(request);
    if (!ok)
    {
        throw std::runtime_error("Stream closed");  //todo: add own exception hierarchy
    }
	
    //closing stream
    stream_->WritesDone();
    samplesStreamCompleted_ = true;

}

RecognizeResponse RemoteSession::WaitForResponse(void)
{
    if (!stream_)
    {
        return RecognizeResponse();
    }
    RecognizeResponse response;
    bool ok = stream_->Read(&response);
    if (!ok)
    {
        auto status = stream_->Finish();
        if (status.error_code() != grpc::OK)
        {
            throw std::runtime_error("Error while reading message: " + status.error_message());
        }
        else
        {
            stream_.reset();
        }
    }
    return response;
}

}
