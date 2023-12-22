#include "Core/Base.hh"
#include <utility>
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
    Ptr<ASTContext> pushContext()
    {
        auto context = makeSharedPtr<ASTContext>(contextStack.size());
        contextStack.push_back(context);
        return context;
    }

    /// 
    /// \brief Pop a context and return from the stack.
    /// 
    Ptr<ASTContext> popContext()
    {
        auto context = contextStack.back();
        contextStack.pop_back();
        return context;
    }

    /// 
    /// \brief Register a `SymbolEntry` at the top context.
    /// 
    void registerSymbol(SymbolEntry::EntrySummary summary_,
                        std::string_view name_, Type type_, bool defined_,
                        const Location *location_, ASTValueType value_);

    /// 
    /// \brief Provide a convenient way to access stack elements
    /// 
    Ptr<ASTContext> operator[](size_t idx)
    {
        return contextStack[contextStack.size() - idx - 1];
    }

    Ptr<const ASTContext> operator[](size_t idx) const
    {
        return contextStack[contextStack.size() - idx - 1];
    }

    std::vector<Ptr<ASTContext>> &getContextStack() { return contextStack; }

    bool contextStackEmpty() const { return contextStack.empty(); }

    size_t contextStackSize() const { return contextStack.size(); }

  protected:
    std::vector<Ptr<ASTContext>> contextStack;
};

} // namespace splc

#endif // __SPLC_AST_ASTCONTEXTMANAGER_HH__