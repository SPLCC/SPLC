#include "Core/Utils/Logging.hh"
#include "Core/Utils/ControlSequence.hh"

#include <cstdlib>
#include <fstream>
#include <iomanip>

namespace splc::utils::logging {

namespace internal {

std::mutex logStreamMutex;

// TODO: consider change logStream
std::ostream *logStream = &std::cerr;

std::ostream &getLogStream() { return *logStream; }

Logger::Logger(const bool enable_, const Level level_) noexcept
    : enable{enable_}, level{level_}, locPtr{nullptr}, trace{false},
      localLogStream{*logStream}
{
    if (!isEnabled())
        return;
    printInitial();
}

Logger::Logger(const bool enable_, const Level level_,
               const Location *const locPtr_, const bool trace_) noexcept
    : enable(enable_), level{level_}, locPtr{locPtr_}, trace{trace_},
      localLogStream{*logStream}
{
    if (!isEnabled())
        return;
    printInitial();
}

void Logger::printInitial() const noexcept
{
    // If the level is empty, don't print any initial banners
    if (level == Level::Empty) {
        return;
    }

    std::lock_guard<std::mutex> lockGuard{logStreamMutex};

    // Trace
    if (trace && locPtr != nullptr && locPtr->parent != nullptr) {
        printLocationStack(locPtr->parent, 0);
    }

    // Header
    localLogStream << ControlSeq::Bold;
    if (locPtr != nullptr && *locPtr) {
        localLogStream << *locPtr;
    }
    else {
        localLogStream << "splc";
    }
    localLogStream << ":" << ControlSeq::Reset << " ";

    // Body
    localLogStream << getLevelColor(level) << level << ControlSeq::Reset
                   << ": ";
}

void printIndicatorUnderline(std::ostream &os, Level level, size_t start,
                             size_t end)
{
    // Indicator
    os << "         | " << ControlSeq::Bold << getLevelColor(level);

    // Space
    for (size_t i = 1; i <= start - 1; ++i) {
        os << " ";
    }

    // Actual indicator
    os << "^";
    for (size_t i = start + 1; i < end; ++i) {
        os << "~";
    }
    os << ControlSeq::Reset;
}

void printIndicatorStr(std::ostream &os, Level level, size_t lineCnt,
                       const std::string &lineStr, size_t start, size_t end)
{
    // Sidebar: line counter
    os << std::setw(8) << std::setfill(' ') << std::right << lineCnt << " | ";

    // Actual content
    os << lineStr.substr(0, start - 1);
    os << ControlSeq::Bold << getLevelColor(level);
    os << lineStr.substr(start - 1, end - start);
    os << ControlSeq::Reset;
    os << lineStr.substr(end - 1);
}

void printIndicator(std::ostream &os, Level level, size_t lineCnt,
                    const std::string &lineStr, size_t start, size_t end)
{
    printIndicatorStr(os, level, lineCnt, lineStr, start, end);
    os << "\n";
    printIndicatorUnderline(os, level, start, end);
}

void printIndicator(std::ostream &os, utils::logging::Level level,
                    const Location &loc)
{
    // TODO: just print more lines
    const std::string &filename = *loc.begin.contextName;
    std::string lineStr;

    // Print a newline to separate from the previous string
    os << "\n";
    std::ifstream fin{filename};
    if (!fin.good()) {
        os << ControlSeq::Bold << ControlSeq::Red
           << "cannot retrieve file: " << ControlSeq::Reset << ControlSeq::Bold
           << *loc.begin.contextName << ControlSeq::Reset;
        return;
    }

    size_t lineCnt = 0;
    while (fin) {
        std::getline(fin, lineStr);
        lineCnt++;
        if (lineCnt == loc.begin.line) {
            break;
        }
    }

    if (lineStr.empty()) {
        os << "cannot retrieve line from location: " << ControlSeq::Bold << loc
           << ControlSeq::Reset;
        return;
    }

    // Print actual error
    size_t start = loc.begin.column, end = 1;
    if (loc.end.line > loc.begin.line) {
        end = 1 + lineStr.length();
    }
    else {
        end = loc.end.column;
    }
    printIndicator(os, level, loc.begin.line, lineStr, start, end);
    // leave the remaining newline to `~Logger()`.
}

Logger::~Logger() noexcept
{
    // End this logstream
    if (isEnabled()) {
        std::lock_guard<std::mutex> lock_guard{logStreamMutex};
        auto loc = locPtr;
        while (loc) {
            if (loc->begin.contextName &&
                loc->begin.traceType == TraceType::FileInclusion) {
                printIndicator(localLogStream, level, *loc);
                break;
            }
            else {
                loc = loc->parent;
            }
            // TODO: support macro expansion
        }
        localLogStream << std::endl;
    }
}

void Logger::printLocationStack(const Location *loc,
                                size_t depth) const noexcept
{
    if (loc != nullptr) {
        if (loc->begin.traceType == TraceType::FileInclusion) {
            if (depth == 0) {
                localLogStream << "In file included from ";
            }
            else {
                localLogStream << "                      ";
            }
            localLogStream << ControlSeq::Bold << loc->begin;
            // If there is still parent left
            if (loc->parent) {
                localLogStream << ",\n";
            }
            else {
                localLogStream << ":\n";
            }
        }
        if (loc->parent) {
            printLocationStack(loc->parent, depth + 1);
        }
    }
}

AssertionHelper::AssertionHelper(bool cond_, const std::string &condText_,
                                 const std::string &file_, int line_,
                                 const std::string &functionName_) noexcept
    : Logger{!cond_, Level::Error}, cond{cond_},
      exitCode{SPLC_EXIT_ASSERTION_FAILURE}
{
    if (!cond) {
        std::lock_guard<std::mutex> lockGuard{logStreamMutex};
        localLogStream << "Assertion failed"
                       << " at " << file_ << ", line " << line_;
        if (!functionName_.empty()) {
            localLogStream << ", at function: " << functionName_;
        }
        localLogStream << "\nCondition evaluated to false: `"
                       << ControlSeq::Bold << condText_ << ControlSeq::Reset
                       << "'";
    }
}

ErrorHelper::ErrorHelper(const std::string &file_, int line_,
                         const std::string &functionName_) noexcept
    : Logger{true, Level::BuiltinError}, exitCode{SPLC_EXIT_ERROR}
{
    std::lock_guard<std::mutex> lockGuard{logStreamMutex};
    localLogStream << "at " << file_ << ", line " << line_;
    if (!functionName_.empty()) {
        localLogStream << ", at function: " << functionName_;
    }
    localLogStream << " ";
}

} // namespace internal

} // namespace splc::utils::logging