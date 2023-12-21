#ifndef __SPLC_TRANSLATION_TRANSLATIONUNIT_HH__
#define __SPLC_TRANSLATION_TRANSLATIONUNIT_HH__ 1

#include <type_traits>

#include "Core/splc.hh"

#include "AST/ASTCommons.hh"
#include "AST/ASTContextManager.hh"

#include "Translation/TranslationBase.hh"
#include "Translation/TranslationOption.hh"
#include "Translation/TranslationContextManager.hh"

namespace splc {

class TranslationUnit;

template <class T>
concept IsTranslationUnit =
    (std::is_same_v<TranslationUnit, std::remove_cvref_t<T>>);

template <class T, class... Functors>
concept AllApplicableOnTranslationUnit = IsTranslationUnit<T> &&
    (std::is_invocable_r_v<T &&, Functors, T &&> &&...);

class TranslationUnit {
  public:
    TranslationUnit()
        : translationContextManager{}, rootNode{}, warningCount{0}, errorCount{
                                                                        0}
    {
    }

    virtual ~TranslationUnit() = default;

    TranslationContextManager &getTranslationContextManager()
    {
        return translationContextManager;
    }

    const TranslationContextManager &getTranslationContextManager() const
    {
        return translationContextManager;
    }

    Ptr<AST> getRootNode() { return rootNode; }

    Ptr<const AST> getRootNode() const { return {rootNode}; }

  protected:
    // TODO: add ASTContextManager?

    /// Manages translation contexts, i.e., file inclusion and macro expansion.
    TranslationContextManager translationContextManager;

    /// Stores the root node of this translation unit.
    Ptr<AST> rootNode;

    int warningCount;
    int errorCount;

    // TODO: add options and includes

  public:
    /// Allow stream-like operation on translation units for processing.
    template <IsTranslationUnit T, class Functor>
    requires AllApplicableOnTranslationUnit<T, Functor>
    friend T &&operator>>(T &&unit, T && (*functor)(T &&));

    /// Allow combined transforms on translation units for processing.
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