#ifndef __SPLC_AST_TOKEN_HH__
#define __SPLC_AST_TOKEN_HH__ 1

#include <iostream>
#include <map>
#include <vector>

#include "parser.hh"

namespace splc::ast {

using SymbolType = Parser::symbol_kind::symbol_kind_type;

inline std::string_view getSymbolName(SymbolType sym)
{
    return Parser::symbol_name(sym);
}

} // namespace splc::ast

#endif /* __SPLC_AST_TOKEN_HH__ */