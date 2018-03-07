#ifndef LIBSARMATA_CLIENT_ASR_SESSION_UTILS_H
#define LIBSARMATA_CLIENT_ASR_SESSION_UTILS_H

#include <iostream>

#include "sarmata_asr.pb.h"

namespace techmo { namespace sarmata {

    std::ostream & operator<<(std::ostream & stream, ResponseStatus status);

}}

#endif
