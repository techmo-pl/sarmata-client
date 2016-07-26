#include "RemoteSession.h"

namespace sarmata
{

RemoteSession::RemoteSession(const std::string & host, const std::string & token, const ASRSessionSettings & settings)
{
    //as
}

void RemoteSession::AddSamples(const std::vector<short> & data)
{

}

void RemoteSession::EndOfStream()
{

}

RecognizeResponse RemoteSession::WaitForResponse(void)
{
    return RecognizeResponse();
}

}
