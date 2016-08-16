#ifndef LIBSARMATA_CLIENT_ASR_SESSION_UTILS_H
#define LIBSARMATA_CLIENT_ASR_SESSION_UTILS_H

#include <iostream>

#include "asr_service.pb.h"

namespace sarmata
{
    std::ostream & operator<<(std::ostream & stream, ResponseStatus status);
}

#endif
