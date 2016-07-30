#ifndef LIBSARMATA_REMOTE_SESSION_H
#define LIBSARMATA_REMOTE_SESSION_H

#include <grpc/grpc.h>
#include <grpc++/channel.h>
#include <grpc++/client_context.h>
#include <grpc++/create_channel.h>
#include <grpc++/security/credentials.h>

#include "asr_service.grpc.pb.h"

#include "ASRSession.h"

namespace sarmata
{
    class RemoteSession: public IASRSession
    {
    public:
        RemoteSession(const std::string & host);
        
        virtual void Open(const std::string & token, const ASRSessionSettings & settings);
        
        virtual void AddSamples(const std::vector<short> & data) override;

        virtual void EndOfStream() override;

        virtual RecognizeResponse WaitForResponse() override;
    private:
        std::string host_;
        std::unique_ptr<ASR::Stub> stub_;
        grpc::ClientContext context_;
        std::unique_ptr<grpc::ClientReaderWriter<RecognizeRequest, RecognizeResponse> > stream_;
    };

}

#endif
