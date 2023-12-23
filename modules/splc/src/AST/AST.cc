#include "AST/AST.hh"

using namespace splc;

Ptr<AST> ASTHelper::getPtrDeclEndPoint(Ptr<AST> ptrRoot) noexcept
{
    splc_assert(ptrRoot->symbolType == ASTSymbolType::PtrDecl);
    AST *tmp = ptrRoot.get();
    while (!tmp->children.empty()) {
        if (tmp->children.back()->symbolType == ASTSymbolType::OpAstrk) {
            return tmp->shared_from_this();
        }
        tmp = tmp->children.back().get();
    }
    splc_unreachable();
}