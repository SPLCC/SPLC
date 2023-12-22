#include "AST/ASTBase.hh"
#include "Core/splc.hh"

#include <algorithm>
#include <iterator>

namespace splc {

Ptr<AST> AST::findFirstChild(ASTSymbolType type) const noexcept
{
    // TODO: find the first available child
    return makeSharedPtr<AST>();
}

Ptr<AST> AST::copy(const std::function<bool(Ptr<const AST>)> &predicate,
                   const bool copyContext) const
{
    Ptr<AST> ret = makeSharedPtr<AST>();
    ret->type = this->type;
    ret->parent = this->parent;

    // Copy child if and only if they satisfy the predicate.
    std::vector<Ptr<AST>> newChildren;
    newChildren.reserve(this->children.size());
    std::copy_if(this->children.begin(), this->children.end(),
                 std::back_inserter(newChildren),
                 [&predicate](Ptr<AST> child) { return predicate(child); });

    std::transform(newChildren.begin(), newChildren.end(),
                   std::back_inserter(ret->children),
                   [&predicate, copyContext](Ptr<AST> node) {
                       return node->copy(predicate, copyContext);
                   });

    ret->loc = this->loc;
    if (copyContext) {
        ret->context = this->context; // TODO(verify): is this desirable?
                                      // Copying the entire context may
                                      // lead to filtered out contents
    }
    ret->value = this->value;

    return ret;
}

Type AST::getType() const
{
    // TODO: type
    return Type::makeType(shared_from_this());
}

Value AST::evaluate()
{
    // TODO: eval
    return Type::makeType(shared_from_this());
}

} // namespace splc