#include "AST/ASTContext.hh"

namespace splc {

bool ASTContext::isSymbolDeclared(SymEntryType symEntTy_,
                                  std::string_view name_) const noexcept
{
    auto it = symbolMap.find(name_);
    return (it != symbolMap.end() && it->second.symEntTy == symEntTy_);
}

bool ASTContext::isSymbolDefined(SymEntryType symEntTy_,
                                 std::string_view name_) const noexcept
{
    auto it = symbolMap.find(name_);
    if (it == symbolMap.end())
        return false;
    else
        return it->second.defined && it->second.symEntTy == symEntTy_;
}

SymbolEntry ASTContext::getSymbol(SymEntryType symEntTy_,
                                  std::string_view name_)
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
    auto it = symbolMap.find(name_);
    if (it != symbolMap.end() && it->second.defined) {
        throw SemanticError{
            &it->second.location,
            "redefining identifier of same type in the same scope"};
    }

    auto symEntry = SymbolEntry::createSymbolEntry(summary_, type_, defined_,
                                                   location_, value_, body_);
    symbolMap.insert_or_assign(ASTIDType{name_}, std::move(symEntry));

    return symEntry;
}

std::ostream &operator<<(std::ostream &os, const ASTContext &ctxt)
{
    using utils::logging::ControlSeq;
    os << ControlSeq::BrightMagenta << "ASTContextTable [" << ctxt.depth << "]"
       << ControlSeq::Reset;
    for (auto &ent : ctxt.symbolMap) {
        os << "    " << ControlSeq::Yellow << ent.first << ControlSeq::Reset
           << ent.second << "\n";
    }
    if (ctxt.symbolMap.empty())
        os << "\n";
    return os;
}

} // namespace splc