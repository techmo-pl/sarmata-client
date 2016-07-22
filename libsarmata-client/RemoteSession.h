#ifndef LIBSARMATA_REMOTE_SESSION_H
#define LIBSARMATA_REMOTE_SESSION_H

#include "ASRSession.h"

namespace sarmata
{
    class RemoteSession: public IASRSession
    {
        RemoteSession(const std::string & token, const ASRSettings & settings);
        virtual void AddSamples(const std::vector<short> & data) override;

        virtual void EndOfStream() override;

        virtual RecognizeResponse WaitForResponse(void) override;

    };

}

#endif
