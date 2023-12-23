#include "AST/ASTContextManager.hh"
#include "AST/ASTCommons.hh"
#include "Core/System.hh"
#include "Core/Utils/LocationWrapper.hh"

namespace splc {

bool ASTContextManager::isSymbolDeclared(std::string_view name_) const noexcept
{
    return contextStack.back()->isSymbolDeclared(name_);
}

bool ASTContextManager::isSymbolDefined(std::string_view name_) const noexcept
{
    return contextStack.back()->isSymbolDefined(name_);
}

SymbolEntry ASTContextManager::getSymbol(std::string_view name_)
{
    return contextStack.back()->getSymbol(name_);
}

SymbolEntry
ASTContextManager::registerSymbol(SymbolEntry::EntrySummary summary_,
                                  std::string_view name_, Type *type_,
                                  bool defined_, const Location *location_,
                                  ASTValueType value_, Ptr<AST> body_)
{
    return contextStack.back()->registerSymbol(summary_, name_, type_, defined_,
                                               location_);
}

} // namespace splc