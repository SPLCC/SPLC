#include "AST/ASTBase.hh"

using namespace splc;

/// Lookup and find the value
ASTValueType ASTHelper::getIDValueMatch(std::string_view name,
                                        const AST &root) noexcept
{
    auto ptr = root.shared_from_this();
    decltype(ptr) nextPtr;

    while (ptr->symbolType != ASTSymbolType::InitDecltr &&
           (nextPtr = ptr->getParent().lock()) != nullptr)
        ptr = nextPtr;

    // If there isn't such one
    if (ptr->symbolType != ASTSymbolType::InitDecltr)
        return {};

    // Else, go inside
    auto it =
        std::find_if(ptr->children.begin(), ptr->children.end(), [](auto &p) {
            return p->symbolType == ASTSymbolType::Initializer;
        });

    if (it == ptr->children.end())
        return {};

    ptr = *it; // ptr is now the initializer.

    // TODO: support initializer-list
    // ASSUMPTION: ONLY ConstExpr in intiializer
    ptr = ptr->children[0]->children[0]->children[0];
    return ptr->value;
}

void ASTHelper::getIDRecursive(std::vector<ASTDeclEntityType> &vec,
                               const AST &root) noexcept
{
    // TODO: revise
    std::for_each(
        root.children.begin(), root.children.end(),
        [&vec](const Ptr<AST> &node) {
            if (node->symbolType == ASTSymbolType::ID) {
                ASTIDType id = node->getValue<ASTIDType>();
                Location loc = node->loc;
                vec.push_back({id, loc, getIDValueMatch(id, *node)});
            }
            else if (isASTSymbolTypeOneOfThem(
                         node->symbolType, ASTSymbolType::FuncDef,
                         ASTSymbolType::FuncDecl, ASTSymbolType::FuncDecltr,
                         ASTSymbolType::DirFuncDecltr, ASTSymbolType::PtrDecl,
                         ASTSymbolType::ID, ASTSymbolType::DirDecl,
                         ASTSymbolType::InitDecltrList,
                         ASTSymbolType::InitDecltr, ASTSymbolType::Decltr,
                         ASTSymbolType::DirDecltr,
                         ASTSymbolType::WrappedDirDecltr)) {
                getIDRecursive(vec, *node);
            }
        });
}

std::vector<ASTDeclEntityType>
ASTHelper::getIDRecursive(const AST &root) noexcept
{
    std::vector<ASTDeclEntityType> results;
    getIDRecursive(results, root);
    return results;
}

std::vector<ASTDeclEntityType> AST::getNamedDeclEntities() const
{
    splc_dbgassert(isASTSymbolTypeOneOfThem(symbolType, ASTSymbolType::Decl,
                                            ASTSymbolType::FuncDecl,
                                            ASTSymbolType::FuncDef));
    return ASTHelper::getIDRecursive(*this);
}