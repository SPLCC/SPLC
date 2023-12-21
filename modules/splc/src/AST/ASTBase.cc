#include "AST/AST.hh"

namespace splc {

Ptr<AST> AST::findFirstChild(ASTSymbolType type) const noexcept
{
    // TODO: find the first available child
    return makeSharedPtr<AST>();
}

Type AST::getType() const noexcept
{
    // TODO: type
    return {shared_from_this()};
}

Value AST::evaluate() const noexcept
{
    // TODO: eval
    return {Type{shared_from_this()}};
}

} // namespace splc