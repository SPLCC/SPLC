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
        All,
        Empty,
        Function,
        StructDecl,
        UnionDecl,
        EnumDecl,
        Typedef,
        Variable,
    };

    SymbolEntry() = delete;

    SymbolEntry(EntrySummary summary_, Type *type_, bool defined_,
                const Location *location_, ASTValueType value_, PtrAST body_)
        : summary{summary_}, type{type_}, defined{defined_},
          location{location_ == nullptr ? Location{} : *location_},
          value{value_}, body{body_}
    {
    }

    ///
    /// \brief Create a new `SymbolEntry`.
    ///
    static SymbolEntry createSymbolEntry(EntrySummary summary, Type *type_,
                                         bool defined_,
                                         const Location *location_,
                                         ASTValueType value_,
                                         PtrAST body = nullptr)
    {
        return {summary, type_, defined_, location_, value_, body};
    }

    constexpr bool hasValue() const noexcept
    {
        return !value.valueless_by_exception();
    }

    template <IsValidASTValue T>
    auto getValue()
    {
        return std::get<T>(value);
    }

    template <class Visitor>
    auto visitValue(Visitor &&vis) const
    {
        return std::visit(vis, value);
    }

    template <IsValidASTValue T>
    void emplaceValue(T &&val)
    {
        value.emplace<T>(std::forward(val));
    }

    template <IsValidASTValue T>
    constexpr bool holdsValueType() const noexcept
    {
        return std::holds_alternative<T>(value);
    }

    // TODO: maybe add some methods

    EntrySummary summary;
    Type *type;
    ASTValueType value; ///< The value of this symbol entry.
                        ///< If this symbol has a value, try const propagation.
    PtrAST body; ///< If this is a function, there stores its body content.

    bool defined; ///< If defined, set to true
    Location location;
};

} // namespace splc

#endif // __SPLC_AST_SYMBOLENTRY_HH__