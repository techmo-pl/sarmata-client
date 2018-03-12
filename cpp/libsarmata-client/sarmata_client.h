#ifndef __SARMATA_CLIENT_H__
#define __SARMATA_CLIENT_H__

#include "sarmata_asr.grpc.pb.h"


namespace techmo { namespace sarmata {

struct SarmataSessionConfig {
    std::string session_id = "";        // Session ID to be passed to the service. If not specified, the service will generate a default session ID itself.
                                        // Session ID is the best way to match log's from client application with these on server side.
    std::string service_settings = "";  // Semicolon-separated list of key=value pairs defining settings to be sent to service via gRPC request.
                                        // For a full list of accepted settings, see `RecognitionConfig`'s description in `proto/sarmata_asr.proto`.
    std::string grammar_name = "";      // Name (ID) of the grammar in the service's grammar cache.
    std::string grammar_data = "";      // SRGS grammar data (ABNF or XML format accepted).
    int max_alternatives = 1;           // Maximum number of recognition hypotheses to be returned.
    double no_match_threshold = 0.0;    // Confidence acceptance threshold.
    int no_input_timeout = 0;           // MRCPv2 No-Input-Timeout in milliseconds.
    int recognition_timeout = 0;        // MRCPv2 Recognition-Timeout in milliseconds.
    int speech_complete_timeout = 0;    // MRCPv2 Speech-Complete-Timeout in milliseconds.
    int speech_incomplete_timeout = 0;  // MRCPv2 Speech-Incomplete-Timeout in milliseconds.
};

class SarmataClient {
public:
    SarmataClient(const std::string& service_address) : service_address_{ service_address } {}

    DefineGrammarResponse DefineGrammar(const SarmataSessionConfig& config) const;

    std::vector<RecognizeResponse> Recognize(const SarmataSessionConfig& config, unsigned int audio_sample_rate_hz, const std::string& audio_byte_content) const;

private:
    SarmataClient(); // Disable default constructor.

    const std::string service_address_;    // IP address and port (address:port) of a service the client will connect to.
};

}}


#endif //__SARMATA_CLIENT_H__
