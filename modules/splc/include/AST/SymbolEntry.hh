#ifndef __SPLC_AST_SYMBOLENTRY_HH__
#define __SPLC_AST_SYMBOLENTRY_HH__ 1

#include "Core/splc.hh"

#include "AST/ASTCommons.hh"
#include "AST/Type.hh"
#include "AST/Value.hh"

namespace splc {

/// \brief `Symbol Entry` describes a variable instance
///        inside SPL language.
class SymbolEntry {
  public:
    enum class EntrySummary {
        Empty,
        Function,
        StructDecl,
        UnionDecl,
        EnumDecl,
        Typedef,
        Variable,
    };

    SymbolEntry() = delete;

    SymbolEntry(bool defined_, Type type_, ASTValueType value_) : defined{defined_}, type{type_}, value{value_} {}

    /// \brief Create a new symbol
    static SymbolEntry createSymbolEntry(bool defined_, Type type_, ASTValueType value_) {
        return {defined_, type_, value_};
    }

    // TODO: maybe add some methods
    bool defined;
    Type type;
    ASTValueType value;
};

} // namespace splc

#endif // __SPLC_AST_SYMBOLENTRY_HH__