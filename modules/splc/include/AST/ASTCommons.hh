#ifndef __SPLC_AST_ASTCOMMONS_HH__
#define __SPLC_AST_ASTCOMMONS_HH__ 1

#include <iostream>
#include <map>
#include <memory>
#include <type_traits>
#include <utility>
#include <variant>
#include <vector>

#include <Core/splc.hh>

#include <AST/ASTSymbol.hh>

namespace splc {

/// class AST forward decl
class AST;

typedef std::string ASTIDType;
typedef char ASTCharType;
typedef unsigned long long ASTIntegralType;
typedef double ASTFloatType;

template <class T>
concept IsASTType = (std::is_base_of_v<AST, std::remove_reference_t<T>>);

template <class T>
concept IsPtrAST = requires(T &&t)
{
    std::static_pointer_cast<AST>(std::forward<T>(t));
};

template <class... Children>
concept AllArePtrAST = (IsPtrAST<Children> && ...);

template <class T, class... Functors>
concept AllApplicableOnAST = IsASTType<T> &&
    (std::is_invocable_r_v<T &&, Functors, T &&> &&...);

template <class T>
concept IsValidASTValue =
    (std::is_same_v<ASTCharType, std::remove_cvref_t<T>> ||
     std::is_same_v<ASTIntegralType, std::remove_cvref_t<T>> ||
     std::is_same_v<ASTFloatType, std::remove_cvref_t<T>> ||
     std::is_same_v<ASTIDType, std::remove_cvref_t<T>>);

template <class T>
auto castToPtrASTBase(T &&t)
{
    return std::static_pointer_cast<AST>(std::forward<T>(t));
}

// template <class Functor>
// concept IsValidASTValueVisitor =
//     (std::is_invocable_v<Functor, char> ||
//      std::is_invocable_v<Functor, unsigned long long> ||
//      std::is_invocable_v<Functor, double> ||
//      std::is_invocable_v<Functor, std::string> ||
//      std::is_invocable_v<Functor, const char> ||
//      std::is_invocable_v<Functor, const unsigned long long> ||
//      std::is_invocable_v<Functor, const double> ||
//      std::is_invocable_v<Functor, const std::string>);

// Value forward decl
class Value;

// SymbolTaEntry forward decl
class SymbolEntry;
using ASTSymbolMap = std::map<ASTIDType, SymbolEntry>;

// SymbolTable forward decl
class SymbolTable;

// ASTContext forward decl
class ASTContext;

// ASTContextManager forward decl
class ASTContextManager;

} // namespace splc

#endif // __SPLC_AST_ASTCOMMONS_HH__