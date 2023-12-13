#include "core/utils/logging.hh"

namespace splc::utils::logging {

namespace internal {

std::mutex logStreamMutex;

// TODO: consider change logStream
std::ostream &logStream = std::cerr;

void handleMessage(const Level level, const location &loc,
                   const std::string &msg, const std::string &exMsg)
{
    std::lock_guard<std::mutex> lockGuard{logStreamMutex};
    // Header
    logStream << ControlSeq::Bold;
    if (loc.end.filename != nullptr) {
        logStream << loc;
    }
    else {
        logStream << "splc";
    }
    logStream << ":" << ControlSeq::Reset << " ";

    // Body
    if (level != Level::Empty) {
        logStream << getLevelColor(level) << level << ControlSeq::Reset
                  << ": ";
    }

    logStream << msg;

    // Extra information
    if (!exMsg.empty()) {
        logStream << " [" << getLevelColor(level) << exMsg
                  << ControlSeq::Reset;
    }

    logStream << "\n";
}

} // namespace internal

} // namespace splc::utils::logging