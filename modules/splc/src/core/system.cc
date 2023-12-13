#include "core/system.hh"

#include <iostream>
#include <sstream>

namespace splc {

RuntimeError::RuntimeError(int code_, const std::string &msg_)
    : std::runtime_error{msg_}, code{code_}, loc{}, msg{msg_}
{
}

RuntimeError::RuntimeError(int code_, const location &loc_,
                           const std::string &msg_)
    : std::runtime_error{msg_}, code{code_}, loc{loc_}, msg{msg_}
{
    formatMessage();
}

const char *RuntimeError::what() const noexcept { return msg.c_str(); }

void RuntimeError::formatMessage()
{
    // Transform multiline messages
    // From OpenCV 4.x
    std::stringstream ss;
    // TODO: format this message
}

} // namespace splc