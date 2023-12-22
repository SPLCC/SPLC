#include "AST/ASTContextManager.hh"
#include "AST/ASTCommons.hh"
#include "Core/System.hh"
#include "Core/Utils/LocationWrapper.hh"

namespace splc {

bool ASTContextManager::isSymbolDeclared(std::string_view name_) const noexcept
{
    auto &map = contextStack.back()->symbolMap;
    return (map.find(name_) != map.end());
}

bool ASTContextManager::isSymbolDefined(std::string_view name_) const noexcept
{
    auto &map = contextStack.back()->symbolMap;
    auto it = map.find(name_);
    if (it == map.end())
        return false;
    else
        return it->second.defined;
}

SymbolEntry ASTContextManager::getSymbol(std::string_view name_)
{
    auto &map = contextStack.back()->symbolMap;
    auto it = map.find(name_);
    if (it == map.end())
        throw SemanticError(nullptr, "trying to get a non-existing symbol");
    return it->second;
}

SymbolEntry
ASTContextManager::registerSymbol(SymbolEntry::EntrySummary summary_,
                                  std::string_view name_, Type *type_,
                                  bool defined_, const Location *location_,
                                  ASTValueType value_, Ptr<AST> body_)
{
    auto &map = contextStack.back()->symbolMap;
    if (auto it = map.find(name_); it != map.end()) {
        throw SemanticError{&it->second.location,
                            "redefining identifier in the same scope"};
    }

    auto symEntry = SymbolEntry::createSymbolEntry(summary_, type_, defined_,
                                                   location_, value_, body_);
    map.insert(std::make_pair(ASTIDType{name_}, std::move(symEntry)));

    return symEntry;
}

} // namespace splc