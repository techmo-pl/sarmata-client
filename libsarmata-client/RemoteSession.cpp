#include "RemoteSession.h"

namespace sarmata
{

RemoteSession::RemoteSession(const std::string & host, std::string & token, const ASRSettings & settings)
{
    //as
}

void RemoteSession::AddSamples(const std::vector<short> & data)
{

}

void RemoteSession::EndOfStream()
{

}

RecognizeResponse WaitForResponse(void)
{
    return RecognizeResponse();
}

}
