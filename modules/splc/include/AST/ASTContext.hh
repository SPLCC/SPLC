#include "Core/Base.hh"
#ifndef __SPLC_AST_ASTCONTEXT_HH__
#define __SPLC_AST_ASTCONTEXT_HH__ 1

#include <string>
#include <string_view>
#include <vector>

#include "Core/splc.hh"

#include "AST/ASTCommons.hh"
#include "AST/SymbolEntry.hh"

namespace splc {

/// \brief `ASTContext` describes declarations in a particular scope.
class ASTContext {
  public:
    ASTContext(ASTContextDepthType depth_) : depth{depth_} {}

    auto &getSymbolMap() { return symbolMap; }

    const auto &getSymbolMap() const { return symbolMap; }

    auto &getSymbolList() { return symbolList; }

    const auto &getSymbolList() const { return symbolList; }

    bool isSymbolDeclared(SymEntryType symEntTy_,
                          std::string_view name_) const noexcept;

    bool isSymbolDefined(SymEntryType symEntTy_,
                         std::string_view name_) const noexcept;

    SymbolEntry getSymbol(SymEntryType symEntTy_, std::string_view name_);

    ///
    /// \brief Register a `SymbolEntry` at the top context.
    ///
    SymbolEntry registerSymbol(SymEntryType summary_, std::string_view name_,
                               Type *type_, bool defined_,
                               const Location *location_,
                               ASTValueType value_ = ASTValueType{},
                               PtrAST body_ = nullptr);

    auto &getParentContexts() { return parentContexts; }

    const auto &getParentContexts() const { return parentContexts; }

    friend std::ostream &operator<<(std::ostream &os, const ASTContext &ctxt);

  protected:
    ASTContextDepthType depth;
    ASTSymbolMap symbolMap;
    std::vector<std::pair<ASTIDType, SymbolEntry>> symbolList; // TODO: just refactor
    std::vector<WeakPtr<ASTContext>> parentContexts;

  public:
    friend class AST;
    friend class ASTProcessor;
    friend class ASTContextManager;
    friend class Type;
    friend class Value;
};

} // namespace splc

#endif // __SPLC_AST_ASTCONTEXT_HH__