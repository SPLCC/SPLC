#include "AST/ASTBase.hh"
#include "AST/ASTContextManager.hh"
#include "Core/Base.hh"
#include "Core/splc.hh"
#include <algorithm>
#include <iterator>

namespace splc {

thread_local ASTPrintMap astPrintMap;

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

std::ostream &operator<<(std::ostream &os, const AST &node) noexcept
{
    using utils::logging::ControlSeq;

    // print node type
    os << ControlSeq::Bold << getASTSymbolColor(node.symType)
       << getASTSymbolName(node.symType) << ControlSeq::Reset;

    // TODO: print node address (allocated)

    // print node location
    os << " <" << ControlSeq::BrightYellow;
    if (auto cid = node.loc.begin.contextID;
        cid != Location::invalidContextID) {
        // TODO: revise this
        if (!astPrintMap.contains(cid)) {
            astPrintMap.insert(cid);
            os << node.loc;
        }
        else {
            os << node.loc.begin.line << "." << node.loc.begin.column << "-";
            os << node.loc.end.line << "." << node.loc.end.column;
        }
    }
    else {
        os << "<invalid sloc>";
    }
    os << ControlSeq::Reset << ">";

    // print node content
    // TODO: print symbol table
    if (node.getContext()) {
        os << " with ";
        const auto &ctx = *node.getContext();
        printASTCtxSummary(os, ctx);
    }

    // print node value
    if (node.hasConstVal()) {
        // template magic
        // TODO: add support for other types
        os << " , val: ";
        os << getASTSymbolColor(node.symType);
        node.visitConstVal(overloaded{
            [&](const auto arg) {}, [&](ASTCharType arg) { os << arg; },
            [&](ASTSIntType arg) { os << arg; },
            [&](ASTUIntType arg) { os << arg; },
            [&](ASTFloatType arg) { os << arg; },
            [&](const ASTIDType &arg) { os << arg; }});
        os << ControlSeq::Reset;
    }

    return os;
}

PtrAST ASTHelper::makeDeclSpecifierTree(const Location &loc,
                                        ASTSymType specSymbolType)
{

    auto intTyNode = AST::make(specSymbolType, loc);
    auto typeSpec = AST::make(ASTSymType::TypeSpec, loc, intTyNode);
    auto declSpec = AST::make(ASTSymType::DeclSpec, loc, typeSpec);
    return declSpec;
}

} // namespace splc