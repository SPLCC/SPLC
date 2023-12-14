#ifndef __SPLC_AST_TOKEN_HH__
#define __SPLC_AST_TOKEN_HH__ 1

#include <iostream>
#include <map>
#include <vector>

#include "core/utils.hh"
#include "parser.hh"

namespace splc::ast {

using SymbolType = Parser::symbol_kind::symbol_kind_type;

inline std::string_view getSymbolName(SymbolType sym)
{
    return Parser::symbol_name(sym);
}

class SymbolColorManipulator {
  public:
    friend SymbolColorManipulator getSymbolColor(SymbolType sym);
    friend std::ostream &operator<<(std::ostream &os,
                                    const SymbolColorManipulator &m);

  private:
    SymbolType sym;
    SymbolColorManipulator(SymbolType sym_) : sym{sym_} {}
};

inline SymbolColorManipulator getSymbolColor(SymbolType sym)
{
    return SymbolColorManipulator{sym};
}

inline std::ostream &operator<<(std::ostream &os,
                                const SymbolColorManipulator &m)
{
    using ControlSeq = utils::logging::ControlSeq;

    os << ControlSeq::Bold;

    switch (m.sym) {
    default:
        break;
    }

    return os;
}

} // namespace splc::ast

#endif /* __SPLC_AST_TOKEN_HH__ */