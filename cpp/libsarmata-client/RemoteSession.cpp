#include "RemoteSession.h"

#include <cstring>

#include <grpc++/client_context.h>
#include <grpc++/create_channel.h>

namespace techmo { namespace sarmata {

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

void RemoteSession::PreDefineGrammar(const std::string & grammarName, const std::string & grammarData)
{
    DefineGrammarRequest request;
    request.set_name(grammarName);
    request.set_grammar(grammarData);

    DefineGrammarResponse response;

    auto stub = ASR::NewStub(grpc::CreateChannel(host_, grpc::InsecureChannelCredentials()));
    grpc::ClientContext context;
    const auto status = stub->DefineGrammar(&context, request, &response);

    if (!status.ok())
    {
        throw std::runtime_error(std::string("DefineGrammar error: ") + status.error_message());
    }

    if (!response.ok())
    {
        throw std::runtime_error(std::string("Grammar not created: ") + response.error());
    }
}

void RemoteSession::Open(const std::string & token, const ASRSessionSettings & settings)
{
    stub_ = ASR::NewStub(grpc::CreateChannel(host_, grpc::InsecureChannelCredentials()));
    stream_ = stub_->Recognize(&context_);
    samplesStreamCompleted_ = false;
    
    RecognitionConfig config;
    for (const auto & field : settings.config)
    {
        auto * configField = config.add_config();
        configField->set_key(field.first);
        configField->set_value(field.second);
    }
    config.set_token(token);
    config.set_sample_rate_hertz(settings.sampleRateHertz);
    config.set_max_alternatives(settings.maxAlternatives);
    if (not settings.grammarName.empty())
    {
        config.set_name(settings.grammarName);
    }
    else
    {
        config.set_data(settings.grammarData);
    }

    RecognizeRequest request;
    *request.mutable_config() = config;

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
	
    const auto chunk_size = 3*1024*1024/sizeof(short);   // less then size in https://github.com/grpc/grpc/blob/v1.0.x/src/core/lib/surface/channel.c#L84
    std::vector<short> chunk;
    chunk.reserve(chunk_size);
    for (int i = 0; i < data.size(); i++)
    {
        chunk.push_back(data[i]);
        if (chunk.size() == chunk_size)
        {
            sendSamples(chunk);
            chunk.clear();
        }
    }
    if (chunk.size() != 0)
    {
        sendSamples(chunk);
    }
}


void RemoteSession::sendSamples(const std::vector<short> & data)
{
    std::string content(data.size() * sizeof(short), 0);
    std::memcpy( (char*)content.data(), data.data(), content.size());
    
    RecognizeRequest request;
    request.set_audio_content(content);
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
	
    //closing stream
    if (not stream_->WritesDone())
    {
        throw std::runtime_error("Stream closed");  //todo: add own exception hierarchy
    }
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

}}
