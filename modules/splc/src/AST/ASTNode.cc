#include "AST/ASTNode.hh"

namespace splc {

Ptr<ASTNode> ASTNode::findFirstChild(ASTSymbolType type) const noexcept
{
    // TODO: find the first available child
    return createPtr<ASTNode>();
}

} // namespace splc