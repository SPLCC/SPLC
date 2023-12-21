#include "AST/ASTContextManager.hh"
#include "AST/ASTCommons.hh"
#include "Core/System.hh"
#include "Core/Utils/LocationWrapper.hh"

namespace splc {

void ASTContextManager::registerSymbol(SymbolEntry::EntrySummary summary_,
                                       std::string_view name_, Type type_,
                                       bool defined_, const Location *location_,
                                       ASTValueType value_)
{
    auto symEntry = SymbolEntry::createSymbolEntry(summary_, type_, defined_,
                                                   location_, value_);

    auto &map = contextStack.back()->symbolMap;
    if (auto it = map.find(name_); it != map.end()) {
        throw SemanticError{&it->second.location,
                            "redefining identifier in the same scope"};
    }
    map.insert(std::make_pair(ASTIDType{name_}, std::move(symEntry)));
}

} // namespace splc