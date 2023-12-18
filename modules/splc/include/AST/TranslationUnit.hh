#include <type_traits>
#ifndef __SPLC_AST_TRANSLATIONUNIT_HH__
#define __SPLC_AST_TRANSLATIONUNIT_HH__ 1

#include "Core/splc.hh"

#include "AST/ASTContextManager.hh"
#include "AST/ASTNode.hh"

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

  private:
    ASTContextManager contextManager;
    Ptr<ASTNode> rootNode;

  public:
    /// Allow stream-like operation on ASTs for processing.
    template <class T, class Functor>
    requires IsTranslationUnit<T> && AllApplicableOnTranslationUnit<T, Functor>
    friend T &&operator>>(T &&unit, T && (*functor)(T &&));

    template <class T, class... Functors>
    requires IsTranslationUnit<T> &&
        AllApplicableOnTranslationUnit<T, Functors...>
    friend T &&applyTranslationUnitTransform(T &&unit, Functors &&...functors);

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

#endif /// __SPLC_AST_TRANSLATIONUNIT_HH__