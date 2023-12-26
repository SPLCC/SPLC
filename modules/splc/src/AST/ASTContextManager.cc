#include "AST/ASTContextManager.hh"
#include "AST/ASTCommons.hh"
#include "Core/Utils/LocationWrapper.hh"

namespace splc {

bool ASTContextManager::isSymDeclared(SymEntryType symEntTy_,
                                      std::string_view name_) const noexcept
{
    return contextStack.back()->isSymDeclared(symEntTy_, name_);
}

bool ASTContextManager::isSymDefined(SymEntryType symEntTy_,
                                     std::string_view name_) const noexcept
{
    return contextStack.back()->isSymDefined(symEntTy_, name_);
}

SymbolEntry ASTContextManager::getSymbol(SymEntryType symEntTy_,
                                         std::string_view name_)
{
    return contextStack.back()->getSymbol(symEntTy_, name_);
}

SymbolEntry ASTContextManager::registerSymbol(SymEntryType summary_,
                                              std::string_view name_,
                                              Type *type_, bool defined_,
                                              const Location *location_,
                                              PtrAST body_)
{
    return contextStack.back()->registerSymbol(summary_, name_, type_, defined_,
                                               location_, body_);
}

} // namespace splc