#ifndef LIBSARMATA_ASRSESSION_H
#define LIBSARMATA_ASRSESSION_H

#include <map>
#include <string>
#include <vector>

#include "asr_service.pb.h"

using ASRSessionSettings = std::map<std::string, std::string>;

namespace sarmata
{
    class IASRSession
    {
    public:
        virtual void Open(const std::string & token, const ASRSessionSettings & settings) = 0;
    
        virtual void AddSamples(const std::vector<short> & data) = 0;

        virtual void EndOfStream() = 0;

        virtual RecognizeResponse WaitForResponse(void) = 0;

    };

}

#endif
