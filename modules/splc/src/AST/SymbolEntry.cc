#include "AST/SymbolEntry.hh"

namespace splc {
    
std::ostream &operator<<(std::ostream &os, const SymbolEntry &ent) noexcept
{
    using utils::logging::ControlSeq;

    os << "SymEntType = " << static_cast<int>(ent.symEntTy) << ", ";
    os << "Type = " << *ent.type << ", ";
    if (ent.hasValue()) {
        os << "Value = ";
        os << ControlSeq::BrightGreen;
        ent.visitValue(overloaded{[&](const auto arg) { os << arg; },
                                  [&](ASTCharType arg) { os << arg; },
                                  [&](ASTSIntType arg) { os << arg; },
                                  [&](ASTUIntType arg) { os << arg; },
                                  [&](ASTFloatType arg) { os << arg; },
                                  [&](const ASTIDType &arg) { os << arg; }});
        os << ControlSeq::Reset << ", ";
    }
    os << "Defined = " << ent.defined << ", ";
    os << "Location = " << ent.location << ";";

    return os;
}

} // namespace splc