#ifndef __SPLC_CORE_UTILS_CONTROL_SEQUENCE_HH__
#define __SPLC_CORE_UTILS_CONTROL_SEQUENCE_HH__ 1

#include <iostream>
#include <string>

#include "../Options.hh"
#include "../System.hh"

extern int splc::options::noDiagnosticColor;

namespace splc::utils::logging {

/** Type of control sequences. If `noDiagnosticColor` is nonzero, then
 * they won't take effect.
 *
 */
enum ControlSeq {
    Black,
    BrightBlack,
    Red,
    BrightRed,
    Green,
    BrightGreen,
    Yellow,
    BrightYellow,
    Blue,
    BrightBlue,
    Magenta,
    BrightMagenta,
    Cyan,
    BrightCyan,
    White,
    BrightWhite,
    Default,
    Bold,
    Reset,
};

static inline std::ostream &operator<<(std::ostream &os, ControlSeq seqType)
{
    if (splc::options::noDiagnosticColor != 0) {
        return os;
    }
    switch (seqType) {
    case Black: {
        os << "\x1B[30m";
        break;
    }
    case BrightBlack: {
        os << "\x1B[90m";
        break;
    }
    case Red: {
        os << "\x1B[31m";
        break;
    }
    case BrightRed: {
        os << "\x1B[91m";
        break;
    }
    case Green: {
        os << "\x1B[32m";
        break;
    }
    case BrightGreen: {
        os << "\x1B[92m";
        break;
    }
    case Yellow: {
        os << "\x1B[33m";
        break;
    }
    case BrightYellow: {
        os << "\x1B[93m";
        break;
    }
    case Blue: {
        os << "\x1B[34m";
        break;
    }
    case BrightBlue: {
        os << "\x1B[94m";
        break;
    }
    case Magenta: {
        os << "\x1B[35m";
        break;
    }
    case BrightMagenta: {
        os << "\x1B[95m";
        break;
    }
    case Cyan: {
        os << "\x1B[36m";
        break;
    }
    case BrightCyan: {
        os << "\x1B[96m";
        break;
    }
    case White: {
        os << "\x1B[37m";
        break;
    }
    case BrightWhite: {
        os << "\x1B[97m";
        break;
    }
    case Default: {
        os << "\x1B[39m";
        break;
    }
    case Bold: {
        os << "\x1B[1m";
        break;
    }
    case Reset: {
        os << "\x1B[0m";
        break;
    }
    default: {
        break;
    }
    }
    return os;
}
} // namespace splc::utils::logging

#endif /* __SPLC_CORE_UTILS_CONTROL_SEQUENCE_HH__ */