#ifndef __SPLC_UTILS_LOGGING_TRACETYPE_HH__
#define __SPLC_UTILS_LOGGING_TRACETYPE_HH__ 1

#include <iostream>
#include <string>

namespace splc::utils::logging {

enum class TraceType {
    Empty,
    MacroVar,
    FileInclusion,
    Struct,
    Function,
};

static inline std::ostream &operator<<(std::ostream &os, TraceType type)
{
    switch (type) {
    case TraceType::MacroVar: {
        os << "macro";
        break;
    }
    case TraceType::FileInclusion: {
        os << "file included";
        break;
    }
    case TraceType::Struct: {
        os << "struct";
        break;
    }
    case TraceType::Function: {
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

#endif // __SPLC_UTILS_LOGGING_TRACETYPE_HH__