#include <type_traits>
#ifndef __SPLC_TRANSLATION_TRANSLATIONUNIT_HH__
#define __SPLC_TRANSLATION_TRANSLATIONUNIT_HH__ 1

#include "Core/splc.hh"

#include "AST/ASTNode.hh"

#include "Translation/TranslationBase.hh"
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
    TranslationUnit() = default;

  protected:
    // TODO: add ASTContextManager?

    /// Manages translation contexts, i.e., file inclusion and macro expansion.
    TranslationContextManager translationContextManager;

    /// Stores the root node of this translation unit.
    Ptr<ASTNode> rootNode;

  public:
    /// Allow stream-like operation on translation units for processing.
    template <class T, class Functor>
    requires IsTranslationUnit<T> && AllApplicableOnTranslationUnit<T, Functor>
    friend T &&operator>>(T &&unit, T && (*functor)(T &&));

    /// Allow combined transforms on translation units for processing.
    template <class T, class... Functors>
    requires IsTranslationUnit<T> &&
        AllApplicableOnTranslationUnit<T, Functors...>
    friend T &&applyTranslationUnitTransform(T &&unit, Functors &&...functors);

    friend class TranslationContextManager;
    friend class TranslationManager;
};

template <class T, class Functor>
requires IsTranslationUnit<T> && AllApplicableOnTranslationUnit<T, Functor>
inline T &&operator>>(T &&unit, Functor &&functor)
{
    return std::forward<Functor>(functor)(std::forward<T>(unit));
}

template <class T, class... Functors>
requires IsTranslationUnit<T> && AllApplicableOnTranslationUnit<T, Functors...>
inline T &&applyTranslationUnitTransform(T &&unit, Functors &&...functors)
{
    return (functors(std::forward<T>(unit)), ...);
}

} // namespace splc

#endif /// __SPLC_TRANSLATION_TRANSLATIONUNIT_HH__