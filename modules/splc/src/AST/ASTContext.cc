#include "AST/ASTContext.hh"

namespace splc {

bool ASTContext::isSymbolDeclared(std::string_view name_) const noexcept
{
    return (symbolMap.find(name_) != symbolMap.end());
}

bool ASTContext::isSymbolDefined(std::string_view name_) const noexcept
{
    auto it = symbolMap.find(name_);
    if (it == symbolMap.end())
        return false;
    else
        return it->second.defined;
}

SymbolEntry ASTContext::getSymbol(std::string_view name_)
{
    auto it = symbolMap.find(name_);
    if (it == symbolMap.end())
        throw SemanticError(nullptr, "trying to get a non-existing symbol");
    return it->second;
}

SymbolEntry ASTContext::registerSymbol(SymbolEntry::EntrySummary summary_,
                                       std::string_view name_, Type *type_,
                                       bool defined_, const Location *location_,
                                       ASTValueType value_, Ptr<AST> body_)
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