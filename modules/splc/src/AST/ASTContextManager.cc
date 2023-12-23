#include "AST/ASTContextManager.hh"
#include "AST/ASTCommons.hh"
#include "Core/System.hh"
#include "Core/Utils/LocationWrapper.hh"

namespace splc {

bool ASTContextManager::isSymbolDeclared(SymEntryType symEntTy_, std::string_view name_) const noexcept
{
    return contextStack.back()->isSymbolDeclared(symEntTy_, name_);
}

bool ASTContextManager::isSymbolDefined(SymEntryType symEntTy_, std::string_view name_) const noexcept
{
    return contextStack.back()->isSymbolDefined(symEntTy_, name_);
}

SymbolEntry ASTContextManager::getSymbol(SymEntryType symEntTy_, std::string_view name_)
{
    return contextStack.back()->getSymbol(symEntTy_, name_);
}

SymbolEntry
ASTContextManager::registerSymbol(SymEntryType summary_,
                                  std::string_view name_, Type *type_,
                                  bool defined_, const Location *location_,
                                  ASTValueType value_, PtrAST body_)
{
    return contextStack.back()->registerSymbol(summary_, name_, type_, defined_,
                                               location_);
}

} // namespace splc