#ifndef __SPLC_UTILS_LOGGING_TRACE_TYPE_HH__
#define __SPLC_UTILS_LOGGING_TRACE_TYPE_HH__ 1

#include <string>

namespace splc::utils::logging {

enum TraceType {
    Macro,
    FileInclusion,
    Struct,
    Function,
};

static inline std::ostream &operator<<(std::ostream &os, TraceType type)
{
    switch (type) {
    case Macro: {
        os << "macro";
        break;
    }
    case FileInclusion: {
        os << "file included";
        break;
    }
    case Struct: {
        os << "struct";
        break;
    }
    case Function: {
        os << "function";
        break;
    }
    default: {
        break;
    }
    }

    return os;
}

} // namespace splc::utils::logging

#endif /* __SPLC_UTILS_LOGGING_TRACE_TYPE_HH__ */