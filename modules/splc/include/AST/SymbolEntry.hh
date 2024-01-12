#ifndef __SPLC_AST_SYMBOLENTRY_HH__
#define __SPLC_AST_SYMBOLENTRY_HH__ 1

#include "AST/ASTCommons.hh"
#include "AST/Value.hh"
#include "Basic/Type.hh"
#include "Core/splc.hh"

namespace splc {

/// \brief `Symbol Entry` describes a variable instance
///        inside SPL language.
class SymbolEntry {
  public:
    SymbolEntry() = delete;

    SymbolEntry(SymEntryType summary_, Type *type_, bool defined_,
                const Location *location_, PtrAST body_)
        : symEntTy{summary_}, type{type_}, defined{defined_},
          location{location_ == nullptr ? Location{} : *location_}, body{body_}
    {
    }

    ///
    /// \brief Create a new `SymbolEntry`.
    ///
    static SymbolEntry createSymbolEntry(SymEntryType summary, Type *type_,
                                         bool defined_,
                                         const Location *location_,
                                         PtrAST body = nullptr)
    {
        return {summary, type_, defined_, location_, body};
    }

    friend std::ostream &operator<<(std::ostream &os,
                                    const SymbolEntry &ent) noexcept;

    // TODO: maybe add some methods

    SymEntryType symEntTy;
    Type *type;
    // ASTValueType value; ///< The value of this symbol entry.
    //                     ///< If this symbol has a value, try const
    //                     propagation.

    bool defined; ///< If defined, set to true
    Location location;

    PtrAST body; ///< If this is a function, there stores its body content.
};

std::ostream &operator<<(std::ostream &os, const SymbolEntry &ent) noexcept;

} // namespace splc

#endif // __SPLC_AST_SYMBOLENTRY_HH__