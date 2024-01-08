#ifndef __SPLC_TRANSLATION_TRANSLATIONUNIT_HH__
#define __SPLC_TRANSLATION_TRANSLATIONUNIT_HH__ 1

#include "AST/ASTCommons.hh"
#include "AST/ASTContextManager.hh"
#include "Basic/SPLCContext.hh"
#include "Core/splc.hh"
#include "Translation/TranslationBase.hh"
#include "Translation/TranslationContextManager.hh"
#include "Translation/TranslationOption.hh"
#include <type_traits>

namespace splc {

class TranslationUnit;

template <class T>
concept IsTranslationUnit =
    (std::is_same_v<TranslationUnit, std::remove_cvref_t<T>>);

template <class T, class... Functors>
concept AllApplicableOnTranslationUnit =
    IsTranslationUnit<T> &&
    (std::is_invocable_r_v<T &&, Functors, T &&> && ...);

class TranslationUnit {
  public:
    TranslationUnit()
        : astCtxMgr{}, transCtxMgr{}, rootNode{}, warningCount{0}, errorCount{0}
    {
        typeContext = makeSharedPtr<SPLCContext>();
    }

    virtual ~TranslationUnit() = default;

    auto getTypeContext() { return typeContext; }

    const auto getTypeContext() const { return typeContext; }

    auto &getASTContextManager() { return astCtxMgr; }

    const auto &getASTContextManager() const { return astCtxMgr; }

    auto &getTranslationContextManager() { return transCtxMgr; }

    const auto &getTranslationContextManager() const { return transCtxMgr; }

    void setRootNode(PtrAST root) { rootNode = root; }

    PtrAST getRootNode() { return rootNode; }

    Ptr<const AST> getRootNode() const { return {rootNode}; }

  protected:
    Ptr<SPLCContext> typeContext; ///< Maintains all internal type contexts.

    ASTContextManager astCtxMgr; ///< Manages AST contexts, i.e., scopes
                                 ///< and variable definitions.

    TranslationContextManager
        transCtxMgr; ///< Manages translation contexts, i.e., file
                     ///< inclusion and macro expansion.

    PtrAST rootNode; ///< Stores the root node of this translation unit.

    int warningCount;
    int errorCount;

    // TODO: add options and includes

  public:
    /// \brief Allow stream-like operation on translation units for processing.
    template <IsTranslationUnit T, class Functor>
        requires AllApplicableOnTranslationUnit<T, Functor>
    friend T &&operator>>(T &&unit, T && (*functor)(T &&));

    /// \brief Allow combined transforms on translation units for processing.
    template <IsTranslationUnit T, class... Functors>
        requires AllApplicableOnTranslationUnit<T, Functors...>
    friend T &&applyTranslationUnitTransform(T &&unit, Functors &&...functors);

  public:
    friend class TranslationContextManager;
    friend class TranslationManager;
    friend class TranslationLogger;
};

template <IsTranslationUnit T, class Functor>
    requires AllApplicableOnTranslationUnit<T, Functor>
inline T &&operator>>(T &&unit, Functor &&functor)
{
    return std::forward<Functor>(functor)(std::forward<T>(unit));
}

template <IsTranslationUnit T, class... Functors>
    requires AllApplicableOnTranslationUnit<T, Functors...>
inline T &&applyTranslationUnitTransform(T &&unit, Functors &&...functors)
{
    return (std::forward<Functors>(functors)(std::forward<T>(unit)), ...);
}

} // namespace splc

#endif /// __SPLC_TRANSLATION_TRANSLATIONUNIT_HH__