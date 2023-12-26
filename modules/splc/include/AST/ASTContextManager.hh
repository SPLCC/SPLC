#include <algorithm>
#include <iterator>
#ifndef __SPLC_AST_ASTCONTEXTMANAGER_HH__
#define __SPLC_AST_ASTCONTEXTMANAGER_HH__ 1

#include <string>
#include <string_view>
#include <vector>

#include "Core/splc.hh"

#include "AST/ASTBase.hh"
#include "AST/ASTCommons.hh"
#include "AST/ASTContext.hh"

namespace splc {

class ASTContextManager {
  public:
    ///
    /// \brief Push a context into the stack.
    ///
    Ptr<ASTContext> pushContext() noexcept
    {
        auto ctxtSz = contextStack.size();
        auto context = makeSharedPtr<ASTContext>(ctxtSz);
        contextStack.push_back(context);
        context->parentContexts.reserve(ctxtSz);
        std::copy(this->contextStack.begin(), this->contextStack.end(), std::back_inserter(context->parentContexts));
        return context;
    }

    ///
    /// \brief Pop a context and return from the stack.
    ///
    Ptr<ASTContext> popContext() noexcept
    {
        auto context = contextStack.back();
        contextStack.pop_back();
        return context;
    }

    bool isSymDeclared(SymEntryType symEntTy_, std::string_view name_) const noexcept;

    bool isSymDefined(SymEntryType symEntTy_, std::string_view name_) const noexcept;

    SymbolEntry getSymbol(SymEntryType symEntTy_, std::string_view name_);

    ///
    /// \brief Register a `SymbolEntry` at the top context.
    ///
    SymbolEntry registerSymbol(SymEntryType summary_,
                               std::string_view name_, Type *type_,
                               bool defined_, const Location *location_,
                               ASTValueType value_ = ASTValueType{},
                               PtrAST body_ = nullptr);

    ///
    /// \brief Provide a convenient way to access stack elements
    ///
    Ptr<ASTContext> operator[](size_t idx)
    {
        return contextStack[contextStack.size() - idx - 1];
    }

    Ptr<const ASTContext> operator[](size_t idx) const noexcept
    {
        return contextStack[contextStack.size() - idx - 1];
    }

    std::vector<Ptr<ASTContext>> &getContextStack() noexcept
    {
        return contextStack;
    }

    const std::vector<Ptr<ASTContext>> &getContextStack() const noexcept
    {
        return contextStack;
    }

    bool contextStackEmpty() const noexcept { return contextStack.empty(); }

    size_t contextStackSize() const noexcept { return contextStack.size(); }

  protected:
    std::vector<Ptr<ASTContext>> contextStack;
};

} // namespace splc

#endif // __SPLC_AST_ASTCONTEXTMANAGER_HH__