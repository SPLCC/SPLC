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
                                       PtrAST body_)
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
                                                   location_, body_);
    auto p = std::make_pair(ASTIDType{name_}, symEntry);
    symbolMap.insert(p);
    symbolList.push_back(p);
    return symEntry;
}

std::ostream &printLeadingSpace(std::ostream &os, ASTContextDepthType depth)
{
    for (ASTContextDepthType i = 0; i < depth; ++i) {
        os << "  ";
    }
    return os;
}

std::ostream &operator<<(std::ostream &os, const ASTContext &ctxt)
{
    using utils::logging::ControlSeq;

    printLeadingSpace(os, ctxt.depth)
        << ControlSeq::BrightMagenta << "ASTContextTable [" << ctxt.depth << "]"
        << ControlSeq::Reset << " at " << ControlSeq::Yellow << &ctxt
        << ControlSeq::Reset << "\n";

    for (auto &ent : ctxt.getSymbolMap()) {
        printLeadingSpace(os, ctxt.depth + 1)
            << ControlSeq::Yellow << ent.first << ControlSeq::Reset << "\n";
        os << ControlSeq::Blue;
        printLeadingSpace(os, ctxt.depth + 1)
            << "`-" << ControlSeq::Reset << ent.second << "\n";
    }
    for (auto &child : ctxt.getDirectChildren()) {
        os << *child;
    }
    return os;
}

} // namespace splc