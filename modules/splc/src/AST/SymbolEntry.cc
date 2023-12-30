#include "AST/SymbolEntry.hh"

namespace splc {

const char *sym_ent_name_[] = {
    "Unspecified", "All",      "Empty",   "Function", "StructDecl",
    "UnionDecl",   "EnumDecl", "Typedef", "Variable", "Parameter",
};

std::ostream &operator<<(std::ostream &os, SymEntryType symEntTy) noexcept
{
    return os << sym_ent_name_[static_cast<size_t>(symEntTy)];
}

std::ostream &operator<<(std::ostream &os, const SymbolEntry &ent) noexcept
{
    using utils::logging::ControlSeq;

    os << "entryTy = " << ControlSeq::BrightCyan << ent.symEntTy
       << ControlSeq::Reset << ", ";

    os << "ty = " << ControlSeq::BrightMagenta << *ent.type << ControlSeq::Reset
       << ", ";

    os << "defined = " << ControlSeq::Green << ent.defined << ControlSeq::Reset
       << ", ";

    os << "loc = " << ent.location << ";";

    return os;
}

} // namespace splc