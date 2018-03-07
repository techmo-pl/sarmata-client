#ifndef LIBSARMATA_ASRSESSION_H
#define LIBSARMATA_ASRSESSION_H

#include <map>
#include <string>
#include <vector>

#include "sarmata_asr.pb.h"

struct ASRSessionSettings
{
    std::string sessionId;
    std::map<std::string, std::string> config;
    int sampleRateHertz = 0;
    int maxAlternatives = 1;
    std::string grammarName;
    std::string grammarData;
    double noMatchThreshold = 0.0;
    int noInputTimeout = 0;
    int recognitionTimeout = 0;
    int speechCompleteTimeout = 0;
    int speechIncompleteTimeout = 0;
};

namespace techmo { namespace sarmata {

    class IASRSession
    {
    public:
        virtual ~IASRSession() {}

        virtual DefineGrammarResponse PreDefineGrammar(const std::string& grammarName, const std::string& grammarData) {};

        virtual void Open(const ASRSessionSettings & settings) = 0;

        virtual void AddSamples(const std::vector<short> & data) = 0;

        virtual void EndOfStream() = 0;

        virtual RecognizeResponse WaitForResponse(void) = 0;

    };

}}

#endif
