#include "AST/ASTProcess.hh"

namespace splc::ASTProcess {

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

} // namespace splc::ASTProcess