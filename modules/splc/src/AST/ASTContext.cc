#include "AST/ASTContext.hh"

namespace splc {

bool ASTContext::isSymDeclared(SymEntryType symEntTy_,
                               std::string_view name_) const noexcept
{
    auto it = symbolMap.find(name_);
    return (it != symbolMap.end() && it->second.symEntTy == symEntTy_);
}

bool ASTContext::isSymDefined(SymEntryType symEntTy_,
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

SymbolEntry ASTContext::registerSymbol(SymEntryType symEntTy_,
                                       std::string_view name_, Type *type_,
                                       bool defined_, const Location *location_,
                                       ASTValueType value_, PtrAST body_)
{
    auto it = symbolMap.find(name_);
    if (it != symbolMap.end()) {
        if (it->second.symEntTy != symEntTy_ || it->second.defined) {
            throw SemanticError{&it->second.location,
                                "redefining same identifier in the same scope"};
        }
        symbolMap.erase(it);
    }

    auto symEntry = SymbolEntry::createSymbolEntry(symEntTy_, type_, defined_,
                                                   location_, value_, body_);
    auto p = std::make_pair(ASTIDType{name_}, symEntry);
    symbolMap.insert(p);
    symbolList.push_back(p);
    return symEntry;
}

std::ostream &operator<<(std::ostream &os, const ASTContext &ctxt)
{
    using utils::logging::ControlSeq;
    os << ControlSeq::BrightMagenta << "ASTContextTable [" << ctxt.depth
       << "]\n"
       << ControlSeq::Reset;
    for (auto &ent : ctxt.symbolMap) {
        os << "  " << ControlSeq::Yellow << ent.first << ControlSeq::Reset
           << "\n";
        os << ControlSeq::Blue << "  `-" << ControlSeq::Reset << ent.second
           << "\n";
    }
    if (ctxt.symbolMap.empty())
        os << "\n";
    return os;
}

} // namespace splc