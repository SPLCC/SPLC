#ifndef __SPLC_AST_AST_PROCESS_HH__
#define __SPLC_AST_AST_PROCESS_HH__ 1

#include <iostream>
#include <map>
#include <vector>

#include "Core/splc.hh"

#include "AST/ASTBase.hh"
#include "AST/ASTNode.hh"

namespace splc {

namespace ASTProcess {

Ptr<ASTNode> removeASTPunctuators(Ptr<ASTNode> node);

Ptr<ASTNode> semanticAnalysis(Ptr<ASTNode> node);

} // namespace ASTProcess

} // namespace splc

#endif /* __SPLC_AST_AST_PROCESS_HH__ */