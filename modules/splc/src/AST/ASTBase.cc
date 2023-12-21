#include "AST/AST.hh"

namespace splc {

Ptr<AST> AST::findFirstChild(ASTSymbolType type) const noexcept
{
    // TODO: find the first available child
    return makeSharedPtr<AST>();
}

} // namespace splc