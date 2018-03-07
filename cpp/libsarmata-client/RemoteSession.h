#ifndef LIBSARMATA_REMOTE_SESSION_H
#define LIBSARMATA_REMOTE_SESSION_H

#include "sarmata_asr.grpc.pb.h"

#include "ASRSession.h"

namespace techmo { namespace sarmata {

    class RemoteSession: public IASRSession
    {
    public:
        RemoteSession(const std::string & host);
        ~RemoteSession();

        DefineGrammarResponse PreDefineGrammar(const std::string & grammarName, const std::string & grammarData) override;

        void Open(const ASRSessionSettings & settings) override;

        void AddSamples(const std::vector<short> & data) override;

        void EndOfStream() override;

        RecognizeResponse WaitForResponse() override;
    private:
        void sendSamples(const std::vector<short> & data);
        
        std::string host_;
        std::unique_ptr<ASR::Stub> stub_;
        grpc::ClientContext context_;
        std::unique_ptr<grpc::ClientReaderWriter<RecognizeRequest, RecognizeResponse> > stream_;
		bool samplesStreamCompleted_;
    };

}}

#endif
