#ifndef LIBSARMATA_ASRSESSION_H
#define LIBSARMATA_ASRSESSION_H

#include <map>
#include <string>
#include <vector>

#include "sarmata_asr.pb.h"

using ASRSessionSettings = std::map<std::string, std::string>;

namespace techmo { namespace sarmata {

    class IASRSession
    {
    public:
        virtual ~IASRSession() {}

        virtual void PreDefineGrammar(const std::string& grammarName, const std::string& grammarData) {};

        virtual void Open(const std::string & token, const ASRSessionSettings & settings) = 0;

        virtual void AddSamples(const std::vector<short> & data) = 0;

        virtual void EndOfStream() = 0;

        virtual RecognizeResponse WaitForResponse(void) = 0;

    };

}}

#endif
