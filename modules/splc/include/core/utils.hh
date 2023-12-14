#ifndef __SPLC_CORE_UTILS_HH__
#define __SPLC_CORE_UTILS_HH__ 1

#include <cstdlib>

#include "utils/cmd_parser.hh"
#include "utils/location_wrapper.hh"
#include "utils/logging.hh"

namespace splc::utils {
// TODO:
} // namespace splc::utils

#define SPLC_EXIT_ERROR             230
#define SPLC_EXIT_ALLOC_FAILURE     232
#define SPLC_EXIT_ASSERTION_FAILURE 233

#ifdef __FUNCTION__
#define __SPLC_LOG_FUNCTION__ ", at function" << __FUNCTION__
#else
#define __SPLC_LOG_FUNCTION__ ""
#endif

#define SPLC_ASSERT(cond)                                                      \
    do {                                                                       \
        if (!(cond)) {                                                         \
            SPLC_LOG_FATAL_ERROR(Location{},                                   \
                                 "Assertion failed at "                        \
                                     << __FILE__ << ", line " << __LINE__      \
                                     << __SPLC_LOG_FUNCTION__ << "\n");        \
            exit(SPLC_EXIT_ASSERTION_FAILURE);                                 \
        }                                                                      \
    } while (0)

// TODO: finish error
#define SPLC_ERROR(msg)                                                        \
    do {                                                                       \
        SPLC_LOG_FATAL_ERROR(Location{},                                       \
                             "Assertion failed at "                            \
                                 << __FILE__ << ", line " << __LINE__          \
                                 << __SPLC_LOG_FUNCTION__ << "\n");            \
        exit(SPLC_EXIT_ERROR);                                                 \
    } while (0)

#endif /* __SPLC_CORE_UTILS_HH__ */