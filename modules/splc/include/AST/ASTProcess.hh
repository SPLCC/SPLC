#ifndef __SPLC_AST_ASTPROCESS_HH__
#define __SPLC_AST_ASTPROCESS_HH__ 1

#include <iostream>
#include <map>
#include <vector>

#include "Core/splc.hh"

#include "AST/AST.hh"
#include "AST/ASTToken.hh"

namespace splc {

namespace ASTProcess {

AST &removeASTPunctuators(AST &node);

} // namespace ASTProcess

} // namespace splc

#endif // __SPLC_AST_ASTPROCESS_HH__