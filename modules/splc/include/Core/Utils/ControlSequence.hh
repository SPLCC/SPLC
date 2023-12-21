#ifndef __SPLC_CORE_UTILS_CONTROLSEQUENCE_HH__
#define __SPLC_CORE_UTILS_CONTROLSEQUENCE_HH__ 1

#include <iostream>
#include <string>

#include "../Options.hh"
#include "../System.hh"

extern int splc::options::noDiagnosticColor;

namespace splc::utils::logging {

/// \brief Type of control sequences.
/// If `noDiagnosticColor` is nonzero, then
/// they won't take effect.
enum class ControlSeq {
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
    case ControlSeq::Black: {
        os << "\x1B[30m";
        break;
    }
    case ControlSeq::BrightBlack: {
        os << "\x1B[90m";
        break;
    }
    case ControlSeq::Red: {
        os << "\x1B[31m";
        break;
    }
    case ControlSeq::BrightRed: {
        os << "\x1B[91m";
        break;
    }
    case ControlSeq::Green: {
        os << "\x1B[32m";
        break;
    }
    case ControlSeq::BrightGreen: {
        os << "\x1B[92m";
        break;
    }
    case ControlSeq::Yellow: {
        os << "\x1B[33m";
        break;
    }
    case ControlSeq::BrightYellow: {
        os << "\x1B[93m";
        break;
    }
    case ControlSeq::Blue: {
        os << "\x1B[34m";
        break;
    }
    case ControlSeq::BrightBlue: {
        os << "\x1B[94m";
        break;
    }
    case ControlSeq::Magenta: {
        os << "\x1B[35m";
        break;
    }
    case ControlSeq::BrightMagenta: {
        os << "\x1B[95m";
        break;
    }
    case ControlSeq::Cyan: {
        os << "\x1B[36m";
        break;
    }
    case ControlSeq::BrightCyan: {
        os << "\x1B[96m";
        break;
    }
    case ControlSeq::White: {
        os << "\x1B[37m";
        break;
    }
    case ControlSeq::BrightWhite: {
        os << "\x1B[97m";
        break;
    }
    case ControlSeq::Default: {
        os << "\x1B[39m";
        break;
    }
    case ControlSeq::Bold: {
        os << "\x1B[1m";
        break;
    }
    case ControlSeq::Reset: {
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

#endif // __SPLC_CORE_UTILS_CONTROLSEQUENCE_HH__