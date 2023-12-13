#ifndef __SPLC_CORE_UTILS_HH__
#define __SPLC_CORE_UTILS_HH__ 1

#include "utils/logging.hh"
#include "utils/cmd_parser.hh"

namespace splc::utils {
// TODO:
} // namespace splc::utils

// TODO: finish this assert
#define SPLC_ASSERT(cond)                                                      \
    do {                                                                       \
        if (!(cond)) {                                                         \
            ;                                                                  \
        }                                                                      \
    } while (0)

// TODO: finish error
#define SPLC_ERROR(msg)                                                        \
    do {                                                                       \
        ;                                                                      \
    } while (0)

#endif /* __SPLC_CORE_UTILS_HH__ */