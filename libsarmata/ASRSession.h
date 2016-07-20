#ifndef LIBSARMATA_ASRSESSION_H
#define LIBSARMATA_ASRSESSION_H

#include <map>
#include <string>
#include <vector>

#include "asr_service.pb.h"

using ASRSettings = std::map<std::string, std::string>;

namespace sarmata
{
    class IASRSession
    {
        virtual void AddSamples(const std::vector<short> & data) = 0;

        virtual void EndOfStream() = 0;

        virtual RecognizeResponse WaitForResponse(void) = 0;

    };

}

#endif
