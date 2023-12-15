#ifndef __SPLC_AST_PROCESS_HH__
#define __SPLC_AST_PROCESS_HH__ 1

#include <iostream>
#include <map>
#include <vector>

#include "Core/splc.hh"

#include "AST/ASTNode.hh"

namespace splc {

namespace ASTProcess {

Ptr<ASTNode> removeASTPunctuators(Ptr<ASTNode> node)
{
    // TODO: Preprocess AST, or just throw away all punctuators at initial
    // (which seems to be a good idea)
    return node;
}

Ptr<ASTNode> semanticAnalysis(Ptr<ASTNode> node)
{
    // TODO: just delete this
    return node;
}

} // namespace ASTProcess

} // namespace splc

#endif /* __SPLC_AST_PROCESS_HH__ */