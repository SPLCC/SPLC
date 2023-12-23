#ifndef __SPLC_AST_ASTCOMMONS_HH__
#define __SPLC_AST_ASTCOMMONS_HH__ 1

#include <AST/ASTSymbol.hh>
#include <AST/TypeContext.hh>
#include <Core/splc.hh>
#include <iostream>
#include <map>
#include <memory>
#include <set>
#include <type_traits>
#include <utility>
#include <variant>
#include <vector>

namespace splc {

typedef unsigned char ASTCharType;
typedef long long ASTSIntType;
typedef unsigned long long ASTUIntType;
typedef double ASTFloatType;
typedef std::string ASTIDType;

using ASTValueType = std::variant<ASTCharType, ASTSIntType, ASTUIntType,
                                  ASTFloatType, ASTIDType>;

template <class T>
concept IsValidASTValue =
    (std::is_same_v<ASTCharType, std::remove_cvref_t<T>> ||
     std::is_same_v<ASTSIntType, std::remove_cvref_t<T>> ||
     std::is_same_v<ASTUIntType, std::remove_cvref_t<T>> ||
     std::is_same_v<ASTFloatType, std::remove_cvref_t<T>> ||
     std::is_same_v<ASTIDType, std::remove_cvref_t<T>>);

// class AST forward decl
class AST;

using PtrAST = Ptr<AST>;
using WeakPtrAST = WeakPtr<AST>;

template <class T>
concept IsBaseAST = (std::is_base_of_v<AST, std::remove_reference_t<T>>);

template <class T>
concept IsPtrAST =
    requires(T &&t) { std::dynamic_pointer_cast<AST>(std::forward<T>(t)); };

template <class... Children>
concept AllArePtrAST = (IsPtrAST<Children> && ...);

template <class T, class... Functors>
concept AllApplicableOnAST =
    IsBaseAST<T> && (std::is_invocable_r_v<T &&, Functors, T &&> && ...);

template <class T>
auto castToPtrASTBase(T &&t)
{
    return std::static_pointer_cast<AST>(std::forward<T>(t));
}

using ASTPrintMap = std::set<utils::Location::ContextIDType>;
extern thread_local ASTPrintMap
    astPrintMap; // TODO: try another way. Internal method.

// ASTHelper forward decl
class ASTHelper;

// ASTProcessor forward decl
class ASTProcessor;

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

enum class SymEntryType {
    All,
    Empty,
    Function,
    StructDecl,
    UnionDecl,
    EnumDecl,
    Typedef,
    Variable,
};

// SymbolEntry forward decl
class SymbolEntry;
using ASTSymbolMap = std::map<ASTIDType, SymbolEntry, std::less<>>;

// SymbolTable forward decl
class SymbolTable;

// ASTContext forward decl
class ASTContext;
using ASTContextDepthType = int;

// ASTContextManager forward decl
class ASTContextManager;

// Translation Unit forward decl
class TranslationUnit;

} // namespace splc

#endif // __SPLC_AST_ASTCOMMONS_HH__