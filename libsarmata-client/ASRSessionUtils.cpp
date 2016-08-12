

#include "asr_service.pb.h"

namespace sarmata
{
    std::ostream & operator<<(std::ostream & stream, ResponseStatus status)
    {
        switch (status)
        {
            case EMPTY                       : stream << "EMPTY"; break;
            case SUCCESS                     : stream << "SUCCESS"; break;
            case PARTIAL_MATCH               : stream << "PARTIAL_MATCH"; break;
            case NO_MATCH                    : stream << "NO_MATCH"; break;
            case NO_INPUT_TIMEOUT            : stream << "NO_INPUT_TIMEOUT"; break;
            case RECOGNITION_TIMEOUT         : stream << "RECOGNITION_TIMEOUT"; break;
            case GRAMMAR_LOAD_FAILURE        : stream << "GRAMMAR_LOAD_FAILURE"; break;
            case GRAMMAR_COMPILATION_FAILURE : stream << "GRAMMAR_COMPILATION_FAILURE"; break;
            case RECOGNIZER_ERROR            : stream << "RECOGNIZER_ERROR"; break;
            case TOO_MUCH_SPEECH_TIMEOUT     : stream << "TOO_MUCH_SPEECH_TIMEOUT"; break;
            case URI_FAILURE                 : stream << "URI_FAILURE"; break;
            case LANGUAGE_UNSUPPORTED        : stream << "LANGUAGE_UNSUPPORTED"; break;
            case CANCELLED                   : stream << "CANCELLED"; break;
            case SEMANTICS_FAILURE           : stream << "SEMANTICS_FAILURE"; break;
            case START_OF_INPUT              : stream << "START_OF_INPUT"; break;
            case END_OF_AUDIO                : stream << "END_OF_AUDIO"; break;
            default                          : stream << "EVENT(" << status << ")"; break;
        }
        return stream;
    }    
}
