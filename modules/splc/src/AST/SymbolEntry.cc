#include "AST/SymbolEntry.hh"

namespace splc {

const char *sym_ent_name_[] = {
    "Undef",     "All",      "Empty",   "Func", "StructDecl",
    "UnionDecl", "EnumDecl", "Typedef", "Var",  "Param",
};

std::ostream &operator<<(std::ostream &os, SymEntryType symEntTy) noexcept
{
    return os << sym_ent_name_[static_cast<size_t>(symEntTy)];
}

std::ostream &operator<<(std::ostream &os, const SymbolEntry &ent) noexcept
{
    using CS = utils::logging::ControlSeq;

    os << CS::BrightCyan << ent.symEntTy << CS::Reset << ", ";

    if (ent.type != nullptr) {
        os << CS::BrightMagenta << *ent.type;
    }
    else {
        os << CS::BrightRed << "nullptr";
    }
    os << CS::Reset << ", ";

    if (ent.defined) {
        os << CS::Green << "defined" << CS::Reset << ", ";
    }
    else {
        os << CS::Blue << "declared" << CS::Reset << ", ";
    }

    os << "at " << ent.location;

    return os;
}

} // namespace splc