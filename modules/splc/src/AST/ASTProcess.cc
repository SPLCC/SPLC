#include "AST/ASTProcess.hh"

namespace splc {

AST &ASTProcessor::removeASTPunctuators(AST &node)
{
    // TODO: Preprocess AST, or just throw away all punctuators at initial
    // (which seems to be a good idea)
    return node;
}

AST &ASTProcessor::reduce(AST &node)
{
    // TODO: reduce AST by eliminating redundant nodes without side-effect
    return node;
}

} // namespace splc