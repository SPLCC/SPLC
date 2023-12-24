#include "AST/SymbolEntry.hh"

namespace splc {

const char *sym_ent_name_[] = {
    "unspecified", "all",      "empty",   "Function", "StructDecl",
    "UnionDecl",   "EnumDecl", "Typedef", "Variable",
};

std::ostream &operator<<(std::ostream &os, SymEntryType symEntTy) noexcept
{
    return os << sym_ent_name_[static_cast<size_t>(symEntTy)];
}

std::ostream &operator<<(std::ostream &os, const SymbolEntry &ent) noexcept
{
    using utils::logging::ControlSeq;

    os << "symEntType = " << ControlSeq::BrightCyan << ent.symEntTy
       << ControlSeq::Reset << ", ";

    os << "type = " << ControlSeq::BrightMagenta << *ent.type
       << ControlSeq::Reset << ", ";

    os << ControlSeq::Yellow;
    os << "[Value = ";
    if (ent.hasValue()) {
        ent.visitValue(overloaded{[&](const auto arg) {},
                                  [&](ASTCharType arg) { os << arg; },
                                  [&](ASTSIntType arg) { os << arg; },
                                  [&](ASTUIntType arg) { os << arg; },
                                  [&](ASTFloatType arg) { os << arg; },
                                  [&](const ASTIDType &arg) { os << arg; }});
    }
    else {
        os << "UNINITIALIZED";
    }
    os << "], " << ControlSeq::Reset;

    os << "defined = " << ControlSeq::Green << ent.defined << ControlSeq::Reset
       << ", ";

    os << "location = " << ent.location << ";";

    return os;
}

} // namespace splc