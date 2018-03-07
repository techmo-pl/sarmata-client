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

DefineGrammarResponse RemoteSession::PreDefineGrammar(const std::string & grammarName, const std::string & grammarData)
{
    DefineGrammarRequest request;
    request.set_grammar_name(grammarName);
    request.set_grammar_data(grammarData);

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

    return response;
}

RecognitionConfig settings_to_config(const ASRSessionSettings & settings)
{
    RecognitionConfig config;

    for (const auto & field : settings.config)
    {
        auto * configField = config.add_additional_settings();
        configField->set_key(field.first);
        configField->set_value(field.second);
    }

    config.set_sample_rate_hertz(settings.sampleRateHertz);
    config.set_max_alternatives(settings.maxAlternatives);

    if (not settings.grammarName.empty())
    {
        config.set_grammar_name(settings.grammarName);
    }
    else
    {
        config.set_grammar_data(settings.grammarData);
    }

    config.set_no_match_threshold(settings.noMatchThreshold);

    auto timeouts = config.mutable_timeout_settings();
    timeouts->set_no_input_timeout(settings.noInputTimeout);
    timeouts->set_recognition_timeout(settings.recognitionTimeout);
    timeouts->set_speech_complete_timeout(settings.speechCompleteTimeout);
    timeouts->set_speech_incomplete_timeout(settings.speechIncompleteTimeout);

    return config;
}

void RemoteSession::Open(const ASRSessionSettings & settings)
{
    stub_ = ASR::NewStub(grpc::CreateChannel(host_, grpc::InsecureChannelCredentials()));
    if (not settings.sessionId.empty()) { context_.AddMetadata("session_id", settings.sessionId); }
    stream_ = stub_->Recognize(&context_);
    samplesStreamCompleted_ = false;

    RecognizeRequest request;
    *request.mutable_config() = settings_to_config(settings);

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
