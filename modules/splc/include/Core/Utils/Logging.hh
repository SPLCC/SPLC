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
concept Streamable = requires(std::ostream &os, T &&t)
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
    /// Default empty constructor that outputs nothing.
    Logger(const bool enable = false) noexcept;
    Logger(const bool enable_, const Level level_,
           const Location *const locPtr_) noexcept;

    Logger(Logger &other) = delete;
    Logger(Logger &&other)
        : enable{other.enable},
          localLogStream{other.localLogStream}, level{other.level}
    {
        other.enable = false;
    }

    Logger &operator=(Logger &other) = delete;

    ~Logger() noexcept;

    template <class T>
    requires Streamable<T> Logger &operator<<(T &&val)
    {
        if (enable) {
            // TODO: switch to full specialization, once gcc supports it
            if constexpr (std::is_same_v<LoggerTag, std::remove_cvref_t<T>>) {
                const LoggerTag &tag = dynamic_cast<const LoggerTag &>(val);
                if (tag.valid()) {
                    localLogStream << " [" << getLevelColor(level) << tag
                                   << ControlSeq::Reset << "]";
                }
            }
            else {
                localLogStream << std::forward<T>(val);
            }
        }
        return *this;
    }

    bool isEnabled() const noexcept { return enable; }

    void setEnabled(bool enable_) noexcept { enable = enable_; }

  protected:
    bool enable;
    std::ostream &localLogStream;
    Level level;
};

// !Mysterious bug from gcc
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

    ~AssertionHelper() noexcept
    {
        if (Logger::isEnabled() && !cond) {
            setEnabled(false);
            localLogStream << std::endl;
            exit(exitCode);
        }
    };

  protected:
    bool cond;
    int exitCode;
};

class ErrorHelper : public Logger {
  public:
    ErrorHelper(int exitCode_) noexcept : Logger{true}, exitCode{exitCode_} {}

    ~ErrorHelper() noexcept
    {
        setEnabled(false);
        localLogStream << std::endl;
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
    splc::utils::logging::internal::Logger {}

#define SPLC_LOG_DISPATCH(level, locPtr)                                       \
    splc::utils::logging::internal::Logger { true, level, locPtr }

#define SPLC_LOG_EMPTY() SPLC_LOG_EMPTY_DISPATCH()

#define SPLC_LOG_DEBUG(locPtr)                                                 \
    SPLC_LOG_DISPATCH(splc::utils::logging::Level::Debug, locPtr)

#define SPLC_LOG_INFO(locPtr)                                                  \
    SPLC_LOG_DISPATCH(splc::utils::logging::Level::Info, locPtr)

#define SPLC_LOG_NOTE(locPtr)                                                  \
    SPLC_LOG_DISPATCH(splc::utils::logging::Level::Note, locPtr)

#define SPLC_LOG_WARN(locPtr)                                                  \
    SPLC_LOG_DISPATCH(splc::utils::logging::Level::Warning, locPtr)

#define SPLC_LOG_ERROR(locPtr)                                                 \
    SPLC_LOG_DISPATCH(splc::utils::logging::Level::Error, locPtr)

#define SPLC_LOG_SYNTAX_ERROR(locPtr)                                          \
    SPLC_LOG_DISPATCH(splc::utils::logging::Level::SyntaxError, locPtr)

#define SPLC_LOG_SEMANTIC_ERROR(locPtr)                                        \
    SPLC_LOG_DISPATCH(splc::utils::logging::Level::SemanticError, locPtr)

#define SPLC_LOG_FATAL_ERROR(locPtr)                                           \
    SPLC_LOG_DISPATCH(splc::utils::logging::Level::FatalError, locPtr)

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