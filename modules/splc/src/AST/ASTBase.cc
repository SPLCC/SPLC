#include "AST/ASTBase.hh"
#include "Core/Base.hh"
#include "Core/splc.hh"

#include <algorithm>
#include <iterator>

using namespace splc;

thread_local ASTPrintMap splc::astPrintMap;

void resetASTPrintMapContext() noexcept { astPrintMap.clear(); }

PtrAST AST::findFirstChild(ASTSymType type) const noexcept
{
    // TODO: find the first available child
    return makeSharedPtr<AST>();
}

PtrAST AST::copy(const std::function<bool(Ptr<const AST>)> &predicate,
                 const bool copyContext) const
{
    PtrAST ret = makeSharedPtr<AST>();
    ret->tyContext = this->tyContext;
    ret->symType = this->symType;
    ret->parent = this->parent;

    // Copy child if and only if they satisfy the predicate.
    std::vector<PtrAST> newChildren;
    newChildren.reserve(this->children_.size());
    std::copy_if(this->children_.begin(), this->children_.end(),
                 std::back_inserter(newChildren),
                 [&predicate](PtrAST child) { return predicate(child); });

    std::transform(newChildren.begin(), newChildren.end(),
                   std::back_inserter(ret->children_),
                   [&predicate, copyContext](PtrAST node) {
                       return node->copy(predicate, copyContext);
                   });

    ret->loc = this->loc;
    if (copyContext) {
        ret->context_ = this->context_; // TODO(verify): is this desirable?
                                        // Copying the entire context may
                                        // lead to filtered out contents
    }
    ret->value = this->value;

    return ret;
}

PtrAST ASTHelper::makeDeclSpecifierTree(const Location &loc,
                                        ASTSymType specSymbolType)
{

    auto intTyNode = AST::make(specSymbolType, loc);
    auto typeSpec = AST::make(ASTSymType::TypeSpec, loc, intTyNode);
    auto declSpec = AST::make(ASTSymType::DeclSpec, loc, typeSpec);
    return declSpec;
}