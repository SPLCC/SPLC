#include "AST/ASTProcess.hh"

namespace splc::ASTProcess {

AST &removeASTPunctuators(AST &node)
{
    // TODO: Preprocess AST, or just throw away all punctuators at initial
    // (which seems to be a good idea)
    return node;
}

} // namespace splc::ASTProcess