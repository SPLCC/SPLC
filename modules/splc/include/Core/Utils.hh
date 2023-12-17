#ifndef __SPLC_CORE_UTILS_HH__
#define __SPLC_CORE_UTILS_HH__ 1

#include <cstdlib>

#include "IO/Parser.hh"
#include "Utils/LocationWrapper.hh"
#include "Utils/Logging.hh"

namespace splc::utils {
} // namespace splc::utils

#define SPLC_EXIT_ERROR             230
#define SPLC_EXIT_ALLOC_FAILURE     232
#define SPLC_EXIT_ASSERTION_FAILURE 233

#ifndef SPLC_NO_FUNCTION_MACRO
#define __SPLC_LOG_FUNCTION_STREAM__ << ", at function: " << __PRETTY_FUNCTION__
#else
#define __SPLC_LOG_FUNCTION__ ""
#endif

#define SPLC_ASSERT(cond)                                                      \
    do {                                                                       \
        if (!(cond)) {                                                         \
            SPLC_LOG_FATAL_ERROR(                                              \
                Location{}, "Assertion ("                                      \
                                << #cond << ") failed at " << __FILE__         \
                                << ", line "                                   \
                                << __LINE__ __SPLC_LOG_FUNCTION_STREAM__);     \
            exit(SPLC_EXIT_ASSERTION_FAILURE);                                 \
        }                                                                      \
    } while (0)

#define SPLC_ERROR(msg)                                                        \
    do {                                                                       \
        SPLC_LOG_FATAL_ERROR(Location{}, msg);                                 \
        exit(SPLC_EXIT_ERROR);                                                 \
    } while (0)

#endif /* __SPLC_CORE_UTILS_HH__ */