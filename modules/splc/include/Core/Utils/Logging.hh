#ifndef __SPLC_UTILS_LOGGING_HH__
#define __SPLC_UTILS_LOGGING_HH__ 1

#include <iostream>
#include <mutex>
#include <sstream>
#include <string>

#include "ControlSequence.hh"
#include "LoggingLevel.hh"
#include "TraceType.hh"

#include "Core/Utils/LocationWrapper.hh"

namespace splc::utils::logging {

namespace internal {

extern std::mutex logStreamMutex;

extern std::ostream *logStream;

std::ostream &getLogStream();

void handleMessage(const Level level, const Location *const locPtr,
                   const std::string &msg, const std::string &exMsg);

} // namespace internal

} // namespace splc::utils::logging

// TODO: check message system macros

#define SPLC_LOG_DISPATCH(level, locPtr, exMsg, ...)                           \
    do {                                                                       \
        std::stringstream ss;                                                  \
        ss << __VA_ARGS__;                                                     \
        std::string msg = ss.str();                                            \
        splc::utils::logging::internal::handleMessage(level, locPtr, msg,      \
                                                      exMsg);                  \
    } while (0)

#define SPLC_LOG_DEBUG(locPtr, ...)                                            \
    SPLC_LOG_DISPATCH(splc::utils::logging::Level::Debug, locPtr, "",          \
                      __VA_ARGS__)

#define SPLC_LOG_INFO(locPtr, ...)                                             \
    SPLC_LOG_DISPATCH(splc::utils::logging::Level::Info, locPtr, "",           \
                      __VA_ARGS__)

#define SPLC_LOG_NOTE(locPtr, ...)                                             \
    SPLC_LOG_DISPATCH(splc::utils::logging::Level::Note, locPtr, "",           \
                      __VA_ARGS__)

#define SPLC_LOG_WARN(locPtr, exMsg, ...)                                      \
    SPLC_LOG_DISPATCH(splc::utils::logging::Level::Warning, locPtr, exMsg,     \
                      __VA_ARGS__)

#define SPLC_LOG_SYNTAX_ERROR(locPtr, ...)                                     \
    SPLC_LOG_DISPATCH(splc::utils::logging::Level::SyntaxError, locPtr, "",    \
                      __VA_ARGS__)

#define SPLC_LOG_SEMANTIC_ERROR(locPtr, ...)                                   \
    SPLC_LOG_DISPATCH(splc::utils::logging::Level::SemanticError, locPtr, "",  \
                      __VA_ARGS__)

#define SPLC_LOG_ERROR(locPtr, ...)                                            \
    SPLC_LOG_DISPATCH(splc::utils::logging::Level::Error, locPtr, "",          \
                      __VA_ARGS__)

#define SPLC_LOG_FATAL_ERROR(locPtr, ...)                                      \
    SPLC_LOG_DISPATCH(splc::utils::logging::Level::FatalError, locPtr, "",     \
                      __VA_ARGS__)

#endif // __SPLC_UTILS_LOGGING_HH__