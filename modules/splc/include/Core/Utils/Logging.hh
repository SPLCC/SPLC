#ifndef __SPLC_UTILS_LOGGING_HH__
#define __SPLC_UTILS_LOGGING_HH__ 1

#include <exception>
#include <iostream>
#include <mutex>
#include <sstream>
#include <string>
#include <type_traits>
#include <utility>

#include "ControlSequence.hh"
#include "LoggingLevel.hh"
#include "TraceType.hh"

#include "Core/Utils/LocationWrapper.hh"

#define SPLC_EXIT_ERROR             230
#define SPLC_EXIT_ALLOC_FAILURE     232
#define SPLC_EXIT_ASSERTION_FAILURE 233

namespace splc::utils::logging {

namespace internal {

class LoggerTag;
class Logger;
class AssertionHelper;
class ErrorHelper;

template <class T>
concept IOStreamable = requires(std::ostream &os, T &&t)
{
    os << std::forward<T>(t);
};

extern std::mutex logStreamMutex;

extern std::ostream *logStream;

std::ostream &getLogStream();

class LoggerTag {
  public:
    LoggerTag(std::string_view msg_) noexcept : msg{msg_} {}

    bool valid() const noexcept { return !msg.empty(); }

  protected:
    std::string_view msg;

  public:
    friend class Logger;

    friend std::ostream &operator<<(std::ostream &os, const LoggerTag &tag);
};

inline std::ostream &operator<<(std::ostream &os, const LoggerTag &tag)
{
    return os << tag.msg;
}

class Logger {
  public:
    ///
    /// \brief Default empty constructor that outputs nothing.
    ///        A default parameter value cannot exist. This is used
    ///        to prevent users from implementing their copy constructors
    ///        incorrectly.
    ///
    Logger(const bool enable, const Level level_ = Level::Empty) noexcept;
    Logger(const bool enable_, const Level level_,
           const Location *const locPtr_, const bool trace_) noexcept;

    Logger(Logger &other) = delete;

    ///
    /// \brief Move constructor that simply disables the output of the logger
    ///        `other`.
    ///
    Logger(Logger &&other)
        : enable{other.enable}, level{other.level}, locPtr{other.locPtr},
          trace{other.trace}, localLogStream{other.localLogStream}

    {
        other.enable = false;
    }

    Logger &operator=(Logger &other) = delete;

    virtual ~Logger() noexcept;

    template <IOStreamable T>
    Logger &operator<<(T &&val);

    bool isEnabled() const noexcept { return enable; }

    void setEnabled(bool enable_) noexcept { enable = enable_; }

  protected:
    virtual void printLocationStack(const Location *loc,
                                    size_t depth) const noexcept;

    ///
    /// \brief Print initial message.
    ///        Implementation of this method must acquire the logstream mutex first.
    /// \example
    /// \code
    /// `splc: error: '
    /// \endcode
    ///
    virtual void printInitial() const noexcept;

    bool enable;
    bool trace;
    std::ostream &localLogStream;
    const Level level;
    const Location *const locPtr;
};

template <IOStreamable T>
inline Logger &Logger::operator<<(T &&val)
{
    if (enable) {
        std::lock_guard<std::mutex> lockGuard{logStreamMutex};

        // TODO: switch to full specialization, once gcc supports it
        if constexpr (std::is_base_of_v<LoggerTag, T>) {
            const LoggerTag &tag = dynamic_cast<const LoggerTag &>(val);
            if (tag.valid()) {
                localLogStream << " [" << getLevelColor(level)
                               << ControlSeq::Bold << tag << ControlSeq::Reset
                               << "]";
            }
        }
        else {
            localLogStream << std::forward<T>(val);
        }
    }
    return *this;
}

// !Mysterious bug in gcc
// template <>
// inline Logger &Logger::operator<<(const LoggerTag &tag)
// {
//     if (tag.valid()) {
//         localLogStream << " [" << getLevelColor(level) << tag << "]"
//                        << ControlSeq::Reset;
//     }
//     return *this;
// }

class AssertionHelper : public Logger {
  public:
    AssertionHelper(bool cond_, const std::string &condText_,
                    const std::string &file_, int line_,
                    const std::string &functionName_) noexcept;
    AssertionHelper(AssertionHelper &other) = delete;
    AssertionHelper(AssertionHelper &&other) : Logger{std::move(other)} {}

    ~AssertionHelper() noexcept
    {
        if (Logger::isEnabled() && !cond) {
            {
                std::lock_guard<std::mutex> lockGuard{logStreamMutex};
                setEnabled(false);
                localLogStream << std::endl;
            }
            exit(exitCode);
        }
    };

  protected:
    bool cond;
    int exitCode;
};

class ErrorHelper : public Logger {
  public:
    ErrorHelper(int exitCode_) noexcept
        : Logger{true, Level::Error}, exitCode{exitCode_}
    {
    }
    ErrorHelper(ErrorHelper &other) = delete;
    ErrorHelper(ErrorHelper &&other) : Logger{std::move(other)} {}

    ~ErrorHelper() noexcept
    {
        {
            std::lock_guard<std::mutex> lockGuard{logStreamMutex};
            setEnabled(false);
            localLogStream << std::endl;
        }
        exit(exitCode);
    };

  protected:
    int exitCode;
};

} // namespace internal

} // namespace splc::utils::logging

// TODO: check message system macros

#define SPLC_LOGGER_TAG(msg)                                                   \
    splc::utils::logging::internal::LoggerTag { msg }

#define SPLC_LOG_EMPTY_DISPATCH()                                              \
    splc::utils::logging::internal::Logger                                     \
    {                                                                          \
        true, splc::utils::logging::Level::Empty                               \
    }

#define SPLC_LOG_DISPATCH(level, locPtr, trace)                                \
    splc::utils::logging::internal::Logger { true, level, (locPtr), (trace) }

#define SPLC_LOG_EMPTY() SPLC_LOG_EMPTY_DISPATCH()

#define SPLC_LOG_DEBUG(locPtr, trace)                                          \
    SPLC_LOG_DISPATCH(splc::utils::logging::Level::Debug, (locPtr), (trace))

#define SPLC_LOG_INFO(locPtr, trace)                                           \
    SPLC_LOG_DISPATCH(splc::utils::logging::Level::Info, (locPtr), (trace))

#define SPLC_LOG_NOTE(locPtr, trace)                                           \
    SPLC_LOG_DISPATCH(splc::utils::logging::Level::Note, (locPtr), (trace))

#define SPLC_LOG_WARN(locPtr, trace)                                           \
    SPLC_LOG_DISPATCH(splc::utils::logging::Level::Warning, (locPtr), (trace))

#define SPLC_LOG_ERROR(locPtr, trace)                                          \
    SPLC_LOG_DISPATCH(splc::utils::logging::Level::Error, (locPtr), (trace))

#define SPLC_LOG_SYNTAX_ERROR(locPtr, trace)                                   \
    SPLC_LOG_DISPATCH(splc::utils::logging::Level::SyntaxError, (locPtr),      \
                      (trace))

#define SPLC_LOG_SEMANTIC_ERROR(locPtr, trace)                                 \
    SPLC_LOG_DISPATCH(splc::utils::logging::Level::SemanticError, (locPtr),    \
                      (trace))

#define SPLC_LOG_FATAL_ERROR(locPtr, trace)                                    \
    SPLC_LOG_DISPATCH(splc::utils::logging::Level::FatalError, (locPtr),       \
                      (trace))

#ifndef SPLC_NO_FUNCTION_MACRO
#define __SPLC_LOG_FUNCTION__ __PRETTY_FUNCTION__
#else
#define __SPLC_LOG_FUNCTION__ ""
#endif

#define SPLC_ASSERT(cond)                                                      \
    splc::utils::logging::internal::AssertionHelper                            \
    {                                                                          \
        cond, #cond, __FILE__, __LINE__, __SPLC_LOG_FUNCTION__                 \
    }

#define SPLC_ERROR()                                                           \
    splc::utils::logging::internal::ErrorHelper {}

#endif // __SPLC_UTILS_LOGGING_HH__