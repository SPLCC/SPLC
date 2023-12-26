#include "AST/ASTBase.hh"

using namespace splc;

void ASTHelper::getIDRecursive(std::vector<ASTDeclEntityType> &vec,
                               const AST &root) noexcept
{
    // TODO: revise
    std::for_each(
        root.children_.begin(), root.children_.end(),
        [&vec](const Ptr<AST> &node) {
            if (node->symType == ASTSymType::ID) {
                ASTIDType id = node->getConstVal<ASTIDType>();
                Location loc = node->loc;
                vec.push_back({id, loc});
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