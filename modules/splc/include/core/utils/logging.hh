#ifndef __SPLC_UTILS_LOGGING_HH__
#define __SPLC_UTILS_LOGGING_HH__ 1

#include <iostream>
#include <mutex>
#include <sstream>
#include <string>

#include "cont_seq.hh"
#include "level.hh"
#include "trace_type.hh"

#include "core/utils/location_wrapper.hh"

namespace splc::utils::logging {

namespace internal {

extern std::mutex logStreamMutex;

extern std::ostream &logStream;

void handleMessage(const Level level, const Location &loc,
                   const std::string &msg, const std::string &exMsg);

} // namespace internal

} // namespace splc::utils::logging

// TODO: check message system macros

#define SPLC_LOG_DISPATCH(level, loc, exMsg, ...)                              \
    do {                                                                       \
        std::stringstream ss;                                                  \
        ss << __VA_ARGS__;                                                     \
        std::string msg = ss.str();                                            \
        splc::utils::logging::internal::handleMessage(level, loc, msg, exMsg); \
    } while (0)

#define SPLC_LOG_DEBUG(loc, ...)                                               \
    SPLC_LOG_DISPATCH(splc::utils::logging::Level::Debug, loc, "", __VA_ARGS__)

#define SPLC_LOG_INFO(loc, ...)                                                \
    SPLC_LOG_DISPATCH(splc::utils::logging::Level::Info, loc, "", __VA_ARGS__)

#define SPLC_LOG_NOTE(loc, ...)                                                \
    SPLC_LOG_DISPATCH(splc::utils::logging::Level::Note, loc, "", __VA_ARGS__)

#define SPLC_LOG_WARN(loc, exMsg, ...)                                         \
    SPLC_LOG_DISPATCH(splc::utils::logging::Level::Warning, loc, exMsg,        \
                      __VA_ARGS__)

#define SPLC_LOG_SYNTAX_ERROR(loc, ...)                                        \
    SPLC_LOG_DISPATCH(splc::utils::logging::Level::SyntaxError, loc, "",       \
                      __VA_ARGS__)

#define SPLC_LOG_SEMANTIC_ERROR(loc, ...)                                      \
    SPLC_LOG_DISPATCH(splc::utils::logging::Level::SemanticError, loc, "",     \
                      __VA_ARGS__)

#define SPLC_LOG_ERROR(loc, ...)                                               \
    SPLC_LOG_DISPATCH(splc::utils::logging::Level::Error, loc, "", __VA_ARGS__)

#define SPLC_LOG_FATAL_ERROR(loc, ...)                                         \
    SPLC_LOG_DISPATCH(splc::utils::logging::Level::FatalError, loc, "",        \
                      __VA_ARGS__)

#endif /* __SPLC_UTILS_LOGGING_HH__ */