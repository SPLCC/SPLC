#ifndef __SPLC_TRANSLATION_TRANSLATIONMANAGER_HH__
#define __SPLC_TRANSLATION_TRANSLATIONMANAGER_HH__ 1

#include <cstddef>
#include <string>
#include <string_view>
#include <vector>

#include "Core/splc.hh"

#include "AST/AST.hh"
#include "AST/ASTContextManager.hh"

#include "Translation/TranslationBase.hh"
#include "Translation/TranslationOption.hh"
#include "Translation/TranslationUnit.hh"

namespace splc {

///
/// \brief Manages the translation process.
/// class `TranslationManager` is designed to decouple the tight cohesion
/// between the driver class and the scanner class. It provides basic facilities
/// such as context switching, error reporting, and stuff.
///
class TranslationManager {
  public:
    TranslationManager() = default;

    virtual ~TranslationManager() = default;

    void startTranslationRecord();

    void endTranslationRecord();

    void reset();

    void setTransUnitRootAST(Ptr<AST> rootNode_)
    {
        tunit->rootNode = rootNode_;
    }

    ///
    /// \brief Create a new node, and add all following children `Ptr<AST>`
    /// to the list of its children.
    ///
    template <IsBaseAST ASTType, AllArePtrAST... Children>
    Ptr<ASTType> makeAST(ASTSymbolType type, const Location &loc,
                         Children &&...children);

    ///
    /// \brief Create a new node, and add all following children `Ptr<AST>`
    /// to the list of its children.
    ///
    template <IsBaseAST ASTType, IsValidASTValue T, AllArePtrAST... Children>
    Ptr<ASTType> makeAST(ASTSymbolType type, const Location &loc, T &&value,
                         Children &&...children);

    auto getTypeContext() { return tunit->typeContext; }

    const auto getTypeContext() const { return tunit->typeContext; }

    Ptr<ASTContext> getCurrentASTContext() noexcept
    {
        return tunit->astContextManager[0];
    }

    void pushASTContext() noexcept { tunit->astContextManager.pushContext(); }

    void popASTContext() noexcept { tunit->astContextManager.popContext(); }

    bool isSymbolDeclared(std::string_view name_) const noexcept
    {
        return tunit->astContextManager.isSymbolDeclared(name_);
    }

    bool isSymbolDefined(std::string_view name_) const noexcept
    {
        return tunit->astContextManager.isSymbolDefined(name_);
    }

    SymbolEntry getSymbol(std::string_view name_);

    SymbolEntry registerSymbol(SymbolEntry::EntrySummary summary_,
                               std::string_view name_, Type *type_,
                               bool defined_, const Location *location_,
                               ASTValueType value_ = ASTValueType{},
                               Ptr<AST> body_ = nullptr);

    Ptr<TranslationContext> getCurrentTransContext() noexcept
    {
        return tunit->translationContextManager[0];
    }

    const std::string &getCurrentTransContextName() const noexcept
    {
        return tunit->translationContextManager[0]->name;
    }

    const TranslationContextIDType getCurrentTransContextID() const noexcept
    {
        return tunit->translationContextManager[0]->contextID;
    }

    TranslationContextKeyType getCurrentTransContextKey() const noexcept
    {
        return tunit->translationContextManager.getCurrentContextKey();
    }

    bool transContextStackEmpty() const
    {
        return tunit->translationContextManager.contextStackEmpty();
    }

    size_t transContextStackSize() const
    {
        return tunit->translationContextManager.contextStackSize();
    }

    ///
    /// \brief Push a new translation context into the stack.
    /// \param intrLoc The location where context switch occurred. Note that
    /// this location
    ///                accepts `nullptr` for representing non-existing
    ///                locations. Internally, a copy is maintained, so that the
    ///                original location will not be affected.
    ///
    Ptr<TranslationContext> pushTransFileContext(const Location *intrLoc_,
                                                 std::string_view fileName_);

    ///
    /// \brief Push a new translation context into the stack.
    /// \param intrLoc The location where context switch occurred. Note that
    /// this location
    ///                accepts `nullptr` for representing non-existing
    ///                locations. Internally, a copy is maintained, so that the
    ///                original location will not be affected.
    ///
    Ptr<TranslationContext>
    pushTransMacroVarContext(const Location *intrLoc_,
                             std::string_view macroVarName_);

    Ptr<TranslationContext> popTransContext() noexcept
    {
        Ptr<TranslationContext> context =
            tunit->translationContextManager.popContext();
        // scanner->yypop_buffer_state();
        return context;
    }

    bool isTransMacroVarPresent(std::string_view macroVarName_) const noexcept
    {
        return tunit->translationContextManager.isTransMacroVarPresent(
            macroVarName_);
    }

    /// \brief Get macro var context from the context manager.
    MacroVarConstEntry
    getTransMacroVarContext(std::string_view macroVarName_) const;

    /// \brief Register a macro variable definition.
    Ptr<TranslationContext>
    registerTransMacroVarContext(const Location *regLocation,
                                 std::string_view macroVarName_,
                                 std::string_view content_);

    /// \brief Unregister a macro variable definition.
    Ptr<TranslationContext>
    unregisterTransMacroVarContext(const Location *unRegLoc,
                                   std::string_view macroVarName_);

    Ptr<TranslationUnit> getTransUnit();

  protected:
    Ptr<TranslationUnit> tunit;

    // TODO: add options
    // TODO: allow manager to retrieve include options and stuff
};

template <IsBaseAST ASTType, AllArePtrAST... Children>
inline Ptr<ASTType> TranslationManager::makeAST(ASTSymbolType type,
                                                const Location &loc,
                                                Children &&...children)
{
    auto parentNode =
        makeAST(tunit->typeContext, loc, std::forward(children)...);
    return parentNode;
}

template <IsBaseAST ASTType, IsValidASTValue T, AllArePtrAST... Children>
inline Ptr<ASTType> TranslationManager::makeAST(ASTSymbolType type,
                                                const Location &loc, T &&value,
                                                Children &&...children)
{
    auto parentNode = makeAST(tunit->typeContext, loc, std::forward(value),
                              std::forward(children)...);
    return parentNode;
}

} // namespace splc

#endif // __SPLC_TRANSLATION_TRANSLATIONMANAGER_HH__