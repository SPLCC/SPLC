#include "AST/ASTBase.hh"

using namespace splc;

/// Lookup and find the value
ASTValueType ASTHelper::getNearestValue(const AST &root) noexcept
{
    auto ptr = root.shared_from_this();
    decltype(ptr) nextPtr;

    while (ptr->sType != ASTSymType::InitDecltr &&
           (nextPtr = ptr->getParent().lock()) != nullptr)
        ptr = nextPtr;

    // If there isn't such one
    if (ptr->sType != ASTSymType::InitDecltr)
        return std::monostate{};

    // Else, go inside
    auto it =
        std::find_if(ptr->children.begin(), ptr->children.end(), [](auto &p) {
            return p->isSymTypeOneOf(ASTSymType::Initializer);
        });

    if (it == ptr->children.end())
        return {};

    ptr = *it; // ptr is now the initializer.

    // TODO: support initializer-list
    // ASSUMPTION: ONLY ConstExpr in intiializer
    auto &exprNode = ptr->children[0];
    for (auto &child : exprNode->children) {
        if (child->getSymType() != ASTSymType::Constant)
            return {};
    }
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
            if (node->sType == ASTSymType::ID) {
                ASTIDType id = node->getValue<ASTIDType>();
                Location loc = node->loc;
                vec.push_back({id, loc, getNearestValue(*node)});
            }
            else if (node->isSymTypeOneOf(
                         ASTSymType::FuncDef, ASTSymType::FuncDecl,
                         ASTSymType::FuncDecltr, ASTSymType::DirFuncDecltr,
                         ASTSymType::PtrDecltr, ASTSymType::ID,
                         ASTSymType::DirDecl, ASTSymType::InitDecltrList,
                         ASTSymType::InitDecltr, ASTSymType::Decltr,
                         ASTSymType::DirDecltr, ASTSymType::WrappedDirDecltr,
                         ASTSymType::ParamTypeList, ASTSymType::ParamList,
                         ASTSymType::ParamDecltr)) {
                getIDRecursive(vec, *node);
            }
        });
}

std::vector<ASTDeclEntityType>
ASTHelper::getNamedDeclRecursive(const AST &root) noexcept
{
    std::vector<ASTDeclEntityType> results;
    getIDRecursive(results, root);
    return results;
}

// TODO(future): get type also
std::vector<ASTDeclEntityType> AST::getNamedDeclEntities() const
{
    splc_dbgassert(isSymTypeOneOf(ASTSymType::Decl, ASTSymType::FuncDecl,
                                  ASTSymType::FuncDef));
    return ASTHelper::getNamedDeclRecursive(*this);
}