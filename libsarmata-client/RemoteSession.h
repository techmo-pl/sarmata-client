#ifndef LIBSARMATA_REMOTE_SESSION_H
#define LIBSARMATA_REMOTE_SESSION_H

#include "ASRSession.h"

namespace sarmata
{
    class RemoteSession: public IASRSession
    {
        public:
        RemoteSession(const std::string & host, const std::string & token, const ASRSessionSettings & settings);
        virtual void AddSamples(const std::vector<short> & data) override;

        virtual void EndOfStream() override;

        virtual RecognizeResponse WaitForResponse() override;
    };

}

#endif
