#include "AST/ASTContext.hh"

namespace splc {

bool ASTContext::isSymbolDeclared(SymEntryType symEntTy_, std::string_view name_) const noexcept
{
    auto it = symbolMap.find(name_);
    return (it != symbolMap.end() && it->second.symEntTy == symEntTy_);
}

bool ASTContext::isSymbolDefined(SymEntryType symEntTy_, std::string_view name_) const noexcept
{
    auto it = symbolMap.find(name_);
    if (it == symbolMap.end())
        return false;
    else
        return it->second.defined && it->second.symEntTy == symEntTy_;
}

SymbolEntry ASTContext::getSymbol(SymEntryType symEntTy_, std::string_view name_)
{
    auto it = symbolMap.find(name_);
    if (it == symbolMap.end() || it->second.symEntTy != symEntTy_)
        throw SemanticError(nullptr, "trying to get a non-existing symbol");
    return it->second;
}

SymbolEntry ASTContext::registerSymbol(SymEntryType summary_,
                                       std::string_view name_, Type *type_,
                                       bool defined_, const Location *location_,
                                       ASTValueType value_, PtrAST body_)
{
    if (auto it = symbolMap.find(name_); it != symbolMap.end()) {
        throw SemanticError{&it->second.location,
                            "redefining identifier in the same scope"};
    }

    auto symEntry = SymbolEntry::createSymbolEntry(summary_, type_, defined_,
                                                   location_, value_, body_);
    symbolMap.insert(std::make_pair(ASTIDType{name_}, std::move(symEntry)));

    return symEntry;
}

} // namespace splc