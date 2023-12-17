#include "Core/Utils/Logging.hh"

namespace splc::utils::logging {

namespace internal {

std::mutex logStreamMutex;

// TODO: consider change logStream
std::ostream *logStream = &std::cerr;

std::ostream &getLogStream()
{
    return *logStream;
}

void handleMessage(const Level level, const Location &loc,
                   const std::string &msg, const std::string &exMsg)
{
    std::lock_guard<std::mutex> lockGuard{logStreamMutex};

    std::ostream &localLogStream = getLogStream();
    // Header
    localLogStream << ControlSeq::Bold;
    if (loc.end.filename != nullptr) {
        localLogStream << loc;
    }
    else {
        localLogStream << "splc";
    }
    localLogStream << ":" << ControlSeq::Reset << " ";

    // Body
    if (level != Level::Empty) {
        localLogStream << getLevelColor(level) << level << ControlSeq::Reset
                  << ": ";
    }

    localLogStream << msg;

    // Extra information
    if (!exMsg.empty()) {
        localLogStream << " [" << getLevelColor(level) << exMsg
                  << ControlSeq::Reset;
    }

    localLogStream << "\n";
}

} // namespace internal

} // namespace splc::utils::logging