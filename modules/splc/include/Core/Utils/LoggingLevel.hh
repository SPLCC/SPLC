#ifndef __SPLC_UTILS_LOGGINGLEVEL_HH__
#define __SPLC_UTILS_LOGGINGLEVEL_HH__ 1

#include <iostream>
#include <string>

#include "Core/Utils/ControlSequence.hh"

namespace splc::utils::logging {

enum class Level {
    Empty,
    Debug,
    Info,
    Note,
    Warning,
    Error,
    SyntaxError,
    SemanticError,
    FatalError,
};

static inline std::ostream &operator<<(std::ostream &os, Level level)
{
    if (level == Level::Empty) {
        return os;
    }

    switch (level) {
    case Level::Info: {
        os << "info";
        break;
    }
    case Level::Debug: {
        os << "debug";
        break;
    }
    case Level::Note: {
        os << "note";
        break;
    }
    case Level::Warning: {
        os << "warning";
        break;
    }
    case Level::Error: {
        os << "error";
        break;
    }
    case Level::SyntaxError: {
        os << "syntax error";
        break;
    }
    case Level::SemanticError: {
        os << "semantic error";
        break;
    }
    case Level::FatalError: {
        os << "fatal error";
        break;
    }
    default: {
        break;
    }
    }

    return os;
}

class LevelColorManipulator {
  public:
    friend LevelColorManipulator getLevelColor(Level level);
    friend std::ostream &operator<<(std::ostream &os,
                                    const LevelColorManipulator &m);

  private:
    LevelColorManipulator(Level level_) : level{level_} {}
    Level level;
};

inline LevelColorManipulator getLevelColor(Level level)
{
    return LevelColorManipulator{level};
}

inline std::ostream &operator<<(std::ostream &os,
                                const LevelColorManipulator &m)
{
    if (m.level == Level::Empty) {
        return os;
    }

    os << ControlSeq::Bold;

    switch (m.level) {
    case Level::Debug: {
        os << ControlSeq::BrightYellow;
        break;
    }
    case Level::Note: {
        os << ControlSeq::BrightCyan;
        break;
    }
    case Level::Warning: {
        os << ControlSeq::BrightMagenta;
        break;
    }
    case Level::Error:
    case Level::SyntaxError:
    case Level::SemanticError:
    case Level::FatalError: {
        os << ControlSeq::BrightRed;
    }

    case Level::Info:
    default:
        break;
    }

    return os;
}

} // namespace splc::utils::logging

#endif // __SPLC_UTILS_LOGGINGLEVEL_HH__