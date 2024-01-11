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
        std::erase_if(symbolList,
                      [&](const auto &sym) { return sym.first == name_; });
    }

    auto symEntry = SymbolEntry::createSymbolEntry(symEntTy_, type_, defined_,
                                                   location_, body_);
    auto p = std::make_pair(ASTIDType{name_}, symEntry);
    symbolMap.insert(p);
    symbolList.push_back(p);
    return symEntry;
}

void ASTContext::unregisterSymbol(SymEntryType entTy, std::string_view name_)
{
    auto it = symbolMap.find(name_);
    if (it != symbolMap.end()) {
        symbolMap.erase(it);
        std::erase_if(symbolList,
                      [&](const auto &sym) { return sym.first == name_; });
    }
}

std::ostream &printLeadingSpace(std::ostream &os, ASTContextDepthType depth)
{
    for (ASTContextDepthType i = 0; i < depth; ++i) {
        os << "  ";
    }
    return os;
}

std::ostream &operator<<(std::ostream &os, const ASTContext &ctx) noexcept
{
    using CS = utils::logging::ControlSeq;

    printLeadingSpace(os, ctx.depth)
        << CS::BrightMagenta << "ASTContextTable [" << ctx.depth << "]"
        << CS::Reset << " at " << CS::Yellow << &ctx << CS::Reset << ", size "
        << CS::Blue << ctx.getSymbolMap().size() << CS::Reset << "\n";

    for (auto &ent : ctx.getSymbolList()) {
        printLeadingSpace(os, ctx.depth + 1)
            << "\"" << CS::BrightGreen << ent.first << CS::Reset << "\", "
            << ent.second << "\n";
    }
    for (auto &child : ctx.getDirectChildren()) {
        os << *child;
    }
    return os;
}

std::ostream &printASTCtxSummary(std::ostream &os,
                                 const ASTContext &ctx) noexcept
{
    using CS = utils::logging::ControlSeq;

    os << CS::BrightMagenta << "ASTCtx [" << ctx.depth << "]" << CS::Reset
       << " at " << CS::Yellow << &ctx << CS::Reset << ", size " << CS::Blue
       << ctx.getSymbolMap().size() << CS::Reset;
    return os;
}

} // namespace splc