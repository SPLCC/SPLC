#ifndef __SPLC_AST_ASTTOKEN_HH__
#define __SPLC_AST_ASTTOKEN_HH__ 1

#include <iostream>

#include "Core/Utils.hh"
#include "IO/Parser.hh"

namespace splc {

using ASTSymbolType = IO::Parser::symbol_kind::symbol_kind_type;

template <class... Types>
concept AllAreASTSymbolType =
    (std::is_same_v<ASTSymbolType,
                    typename std::remove_reference<Types>::type> &&
     ...);

template <AllAreASTSymbolType... SourceTypes>
bool isASTSymbolTypeOneOfThem(ASTSymbolType type, SourceTypes &&...otherTypes)
{
    return ((type == otherTypes) || ...);
}

inline std::string_view getASTSymbolName(ASTSymbolType sym)
{
    return IO::Parser::symbol_name(sym);
}

class ASTSymbolColorManipulator {
  public:
    friend ASTSymbolColorManipulator getASTSymbolColor(ASTSymbolType sym);
    friend std::ostream &operator<<(std::ostream &os,
                                    const ASTSymbolColorManipulator &m);

  private:
    ASTSymbolType sym;
    ASTSymbolColorManipulator(ASTSymbolType sym_) : sym{sym_} {}
};

inline ASTSymbolColorManipulator getASTSymbolColor(ASTSymbolType sym)
{
    return ASTSymbolColorManipulator{sym};
}

inline std::ostream &operator<<(std::ostream &os,
                                const ASTSymbolColorManipulator &m)
{
    using ControlSeq = utils::logging::ControlSeq;

    os << ControlSeq::Bold << ControlSeq::BrightGreen;

    switch (m.sym) {
    default:
        // TODO: color symbols
        break;
    }

    return os;
}

} // namespace splc

#endif // __SPLC_AST_ASTTOKEN_HH__