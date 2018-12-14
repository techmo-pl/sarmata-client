#include <sstream>
#include <atomic>
#include <thread>

#include <grpc++/grpc++.h>

#include "sarmata_asr.grpc.pb.h"
#include "sarmata_client.h"


namespace techmo { namespace sarmata {

// Forward declarations
void prepare_context(grpc::ClientContext& context, const SarmataSessionConfig& config);
std::vector<RecognizeRequest> build_request(const SarmataSessionConfig& config, const std::string& audio_byte_content);
bool error_response(const RecognizeResponse& response);
std::string grpc_status_to_string(const grpc::Status& status);


DefineGrammarResponse SarmataClient::DefineGrammar(const SarmataSessionConfig& config) const {
    auto stub = ASR::NewStub(grpc::CreateChannel(service_address_, grpc::InsecureChannelCredentials()));

    DefineGrammarRequest request;
    request.set_grammar_name(config.grammar_name);
    request.set_grammar_data(config.grammar_data);

    DefineGrammarResponse response;

    grpc::ClientContext context;
    prepare_context(context, config);

    const grpc::Status status = stub->DefineGrammar(&context, request, &response);

    if (not status.ok())
    {
        std::cerr << "DefineGrammar RPC failed with status " << grpc_status_to_string(status) << std::endl;
    }

    return response;
}


std::vector<RecognizeResponse> SarmataClient::Recognize(SarmataSessionConfig& config, unsigned int audio_sample_rate_hz, const std::string& audio_byte_content) const {
    auto stub = ASR::NewStub(grpc::CreateChannel(service_address_, grpc::InsecureChannelCredentials()));

    grpc::ClientContext context;
    prepare_context(context, config);

    auto stream = stub->Recognize(&context);

    config.audio_sample_rate_hz = audio_sample_rate_hz;
    const auto requests = build_request(config, audio_byte_content);

    const auto& config_request = requests.front();
    stream->Write(config_request);

    // When received an error response from the server, the server will not process
    // additional audio (although it may subsequently return additional results).
    // The client should stop sending additional audio, half-close the gRPC connection,
    // and wait for any additional results until the server closes the gRPC connection.
    std::atomic<bool> half_closed_stream{false};

    std::thread writer([&half_closed_stream, &stream, &requests] {
        for (auto i = 1; i < requests.size(); ++i) {
            if (half_closed_stream or not stream->Write(requests[i])) {
                break;
            }
        }
        if (not half_closed_stream) {
            half_closed_stream = true;
            stream->WritesDone();
        }
    });

    const auto responses = [&half_closed_stream, &stream, &writer] {
        std::vector<RecognizeResponse> received_responses;
        RecognizeResponse received_response;
        while (stream->Read(&received_response)) {
            if (error_response(received_response)) {
                if (not half_closed_stream) {
                    half_closed_stream = true;
                    stream->WritesDone();
                }
            }
            else {
                std::cout << "Received response." << std::endl;
            }
            received_responses.push_back(received_response);
        }
        if (writer.joinable()) {
            writer.join();
        }
        return received_responses;
    }();

    const grpc::Status status = stream->Finish();

    if (not status.ok()) {
        std::cerr << "StreamingRecognize RPC failed with status " << grpc_status_to_string(status) << std::endl;
    }

    return responses;
}


void prepare_context(grpc::ClientContext& context, const SarmataSessionConfig& config) {
    if (not config.session_id.empty()) {
        context.AddMetadata("session_id", config.session_id);
    }
    if (config.grpc_timeout > 0) {
        context.set_deadline(std::chrono::system_clock::now() + std::chrono::milliseconds{ config.grpc_timeout });
    }
}

void fill_additional_settings(const SarmataSessionConfig& config, RecognitionConfig& recognition_config) {
    for (const auto& entry : config.service_settings) {
        auto field = recognition_config.add_additional_settings();
        field->set_key(entry.first);
        field->set_value(entry.second);
    }
}

void build_recognition_config(const SarmataSessionConfig& config, RecognitionConfig& recognition_config) {
    recognition_config.set_sample_rate_hertz(config.audio_sample_rate_hz);
    recognition_config.set_max_alternatives(config.max_alternatives);
    recognition_config.set_no_match_threshold(config.no_match_threshold);

    if (not config.grammar_name.empty()) {
        recognition_config.set_grammar_name(config.grammar_name);
    }
    else {
        recognition_config.set_grammar_data(config.grammar_data);
    }

    auto timeouts = recognition_config.mutable_timeout_settings();
    timeouts->set_no_input_timeout(config.no_input_timeout);
    timeouts->set_recognition_timeout(config.recognition_timeout);
    timeouts->set_speech_complete_timeout(config.speech_complete_timeout);
    timeouts->set_speech_incomplete_timeout(config.speech_incomplete_timeout);

    if (not config.service_settings.empty()) {
        fill_additional_settings(config, recognition_config);
    }
}

std::vector<RecognizeRequest> build_request(const SarmataSessionConfig& config, const std::string& audio_byte_content)
{
    RecognizeRequest request;
    build_recognition_config(config, *request.mutable_config());

    std::vector<RecognizeRequest> requests;
    requests.push_back(request);

    unsigned int frame_length = 20;//milliseconds [ms]
    unsigned int frame_size = frame_length * config.audio_sample_rate_hz / 1000;//samples
    for (auto i = 0; i < audio_byte_content.length(); i += frame_size)
    {
        RecognizeRequest request;

        request.set_audio_content(audio_byte_content.substr(i,frame_size));
        requests.push_back(request);
    }

    return requests;
}

bool error_response(const RecognizeResponse& response) {
    const auto status = response.status();
    const auto is_error = (status == GRAMMAR_LOAD_FAILURE
                        || status == GRAMMAR_COMPILATION_FAILURE
                        || status == RECOGNIZER_ERROR
                        || status == SEMANTICS_FAILURE);

    if (is_error) {
        const auto& error = response.error();
        std::cerr << "Received error response: (" << ResponseStatus_Name(status) << ") " << error << std::endl;
    }

    return is_error;
}

std::string grpc_status_to_string(const grpc::Status& status) {
    // Status codes and their use in gRPC explanation can be found here:
    // https://github.com/grpc/grpc/blob/master/doc/statuscodes.md
    // https://grpc.io/grpc/cpp/namespacegrpc.html#aff1730578c90160528f6a8d67ef5c43b
    const std::string status_string = [&status]() {
        using code = grpc::StatusCode;
        switch (status.error_code()) {
        // Based on https://grpc.io/grpc/cpp/impl_2codegen_2status__code__enum_8h_source.html
            case code::OK: return "OK";
            case code::CANCELLED: return "CANCELLED";
            case code::UNKNOWN: return "UNKNOWN";
            case code::INVALID_ARGUMENT: return "INVALID_ARGUMENT";
            case code::DEADLINE_EXCEEDED: return "DEADLINE_EXCEEDED";
            case code::NOT_FOUND: return "NOT_FOUND";
            case code::ALREADY_EXISTS: return "ALREADY_EXISTS";
            case code::PERMISSION_DENIED: return "PERMISSION_DENIED";
            case code::UNAUTHENTICATED: return "UNAUTHENTICATED";
            case code::RESOURCE_EXHAUSTED: return "RESOURCE_EXHAUSTED";
            case code::FAILED_PRECONDITION: return "FAILED_PRECONDITION";
            case code::ABORTED: return "ABORTED";
            case code::OUT_OF_RANGE: return "OUT_OF_RANGE";
            case code::UNIMPLEMENTED: return "UNIMPLEMENTED";
            case code::INTERNAL: return "INTERNAL";
            case code::UNAVAILABLE: return "UNAVAILABLE";
            case code::DATA_LOSS: return "DATA_LOSS";
            case code::DO_NOT_USE: return "DO_NOT_USE";
            default: return "Status code not recognized";
        }
    }();

    return status_string + " (" + std::to_string(status.error_code()) + ") " + status.error_message();
}

}}
