#include <cstdlib>

#include "Core/Utils/Logging.hh"

namespace splc::utils::logging {

namespace internal {

std::mutex logStreamMutex;

// TODO: consider change logStream
std::ostream *logStream = &std::cerr;

std::ostream &getLogStream() { return *logStream; }

Logger::Logger(const bool enable_) noexcept
    : enable{enable_}, localLogStream{*logStream}, level{Level::Empty}
{
}

Logger::Logger(const bool enable_, const Level level_,
               const Location *const locPtr_) noexcept
    : enable(enable_), localLogStream{*logStream}, level{level_}
{
    std::lock_guard<std::mutex> lockGuard{logStreamMutex};

    // Header
    localLogStream << ControlSeq::Bold;
    if (locPtr_ != nullptr && locPtr_->end.filename != nullptr) {
        localLogStream << *locPtr_;
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
}

Logger::~Logger() noexcept
{
    // End this logstream
    localLogStream << std::endl;

    if (level >= Level::Error) {
        std::exit(EXIT_FAILURE);
    }
}

AssertionHelper::AssertionHelper(bool cond_, const std::string &condText_,
                                 const std::string &file_, int line_,
                                 const std::string &functionName_) noexcept
    : Logger{!cond_}, cond{cond_}, exitCode{SPLC_EXIT_ASSERTION_FAILURE}
{
    if (!cond) {
        localLogStream << "Assertion failed at " << file_ << ", line " << line_;
        if (!functionName_.empty()) {
            localLogStream << ", at function: " << functionName_;
        }
        localLogStream << "\n    " << condText_;
    }
}

} // namespace internal

} // namespace splc::utils::logging