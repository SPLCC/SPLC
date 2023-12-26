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
    ret->typeContext = this->typeContext;
    ret->sType = this->sType;
    ret->parent = this->parent;

    // Copy child if and only if they satisfy the predicate.
    std::vector<PtrAST> newChildren;
    newChildren.reserve(this->children.size());
    std::copy_if(this->children.begin(), this->children.end(),
                 std::back_inserter(newChildren),
                 [&predicate](PtrAST child) { return predicate(child); });

    std::transform(newChildren.begin(), newChildren.end(),
                   std::back_inserter(ret->children),
                   [&predicate, copyContext](PtrAST node) {
                       return node->copy(predicate, copyContext);
                   });

    ret->loc = this->loc;
    if (copyContext) {
        ret->astContext = this->astContext; // TODO(verify): is this desirable?
                                            // Copying the entire context may
                                            // lead to filtered out contents
    }
    ret->value = this->value;

    return ret;
}

Value AST::evaluate()
{
    // TODO: eval
    return {nullptr};
}

Ptr<AST> ASTHelper::getPtrDeclEndPoint(AST &root) noexcept
{
    splc_assert(root.sType == ASTSymType::PtrDecltr);

    // Use a bit hack here to remove constness
    AST *tmp = &root;
    while (!tmp->children.empty()) {
        if (tmp->children.back()->sType != ASTSymType::PtrDecltr) {
            return tmp->shared_from_this();
        }
        tmp = tmp->children.back().get();
    }

    splc_unreachable();
}

PtrAST ASTHelper::makeDeclSpecifierTree(const Location &loc,
                                        ASTSymType specSymbolType)
{

    auto intTyNode = makeAST<AST>(specSymbolType, loc);
    auto typeSpec = makeAST<AST>(ASTSymType::TypeSpec, loc, intTyNode);
    auto declSpec = makeAST<AST>(ASTSymType::DeclSpec, loc, typeSpec);
    return declSpec;
}