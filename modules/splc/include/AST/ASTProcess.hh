#ifndef __SPLC_AST_AST_PROCESS_HH__
#define __SPLC_AST_AST_PROCESS_HH__ 1

#include <iostream>
#include <map>
#include <vector>

#include "Core/splc.hh"

#include "AST/ASTNode.hh"
#include "AST/ASTToken.hh"

namespace splc {

namespace ASTProcess {

ASTNode &removeASTPunctuators(ASTNode &node);

} // namespace ASTProcess

} // namespace splc

#endif // __SPLC_AST_AST_PROCESS_HH__