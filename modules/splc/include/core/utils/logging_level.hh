#ifndef __SPLC_UTILS_LOGGING_LEVEL_HH__
#define __SPLC_UTILS_LOGGING_LEVEL_HH__ 1

#include <string>

namespace splc::utils::logging {

enum Level {
    Empty,
    Debug,
    Info,
    Note,
    Warning,
    SyntaxError,
    SemanticError,
    Error,
    FatalError,
};

static inline std::ostream &operator<<(std::ostream &os, Level level)
{
    if (level == Level::Empty) {
        return os;
    }

    switch (level) {
    case Info: {
        os << "info";
        break;
    }
    case Debug: {
        os << "debug";
        break;
    }
    case Note: {
        os << "note";
        break;
    }
    case Warning: {
        os << "warning";
        break;
    }
    case SyntaxError: {
        os << "syntax error";
        break;
    }
    case SemanticError: {
        os << "semantic error";
        break;
    }
    case Error: {
        os << "error";
        break;
    }
    case FatalError: {
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
    friend inline LevelColorManipulator getLevelColor(Level level);

    Level level;

  private:
    LevelColorManipulator(Level level_) : level{level_} {}
};

static inline std::ostream &operator<<(std::ostream &os,
                                       const LevelColorManipulator &m)
{
    if (m.level == Level::Empty) {
        return os;
    }

    os << ControlSeq::Bold;

    switch (m.level) {
    case Debug: {
        os << ControlSeq::BrightYellow;
        break;
    }
    case Note: {
        os << ControlSeq::BrightCyan;
        break;
    }
    case Warning: {
        os << ControlSeq::BrightMagenta;
        break;
    }
    case SyntaxError:
    case SemanticError:
    case Error:
    case FatalError: {
        os << ControlSeq::BrightRed;
    }

    case Info:
    default:
        break;
    }

    return os;
}

inline LevelColorManipulator getLevelColor(Level level)
{
    return LevelColorManipulator{level};
}

} // namespace splc::utils::logging

#endif /* __SPLC_UTILS_LOGGING_LEVEL_HH__ */