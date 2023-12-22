#ifndef __SPLC_AST_TYPE_HH__
#define __SPLC_AST_TYPE_HH__ 1

#include <vector>

#include "Basic/Specifiers.hh"
#include "Core/splc.hh"

#include "AST/ASTCommons.hh"

namespace splc {

/// \brief
enum class TypeCastResult {

};

/// \brief `Type` describes the type of a symbol inside SPL.
class Type {
  public:
    // TODO: maybe refactor this
    Type() = delete;

    ///
    /// \brief Construct a type. A deep copy of the given declaration is used 
    ///        to construct the actual type.
    ///
    static Type makeType(Ptr<const AST> declRoot);

    ///
    /// \brief Compare two types. If they are equivalent, return true.
    ///
    static bool compareType(Type t1, Type t2);

    ///
    /// \brief Compare two types. If they are equivalent, return true.
    ///
    bool operator==(const Type &t2) { return compareType(*this, t2); }

    ///
    /// \brief Get the subtype of the given type.
    /// \example The direct subtype of type `int **p[3]` is `int **pp`, because
    ///          the precendece of array subscription operator is higher than
    ///          that of the indirection operator.
    /// \example The direct subtype of type `int **(*func)(int k, int v)` is
    ///          `int **`, as calling `(*func)(int, int)` returns a second-level
    ///          pointer.
    ///
    static Type getSubType(Type t1);

    Type getSubType();

    ///
    /// \brief Get the decayed type of type.
    /// The decayed type of type `T` is the true type of a parameter that a
    /// function receives when an instance of `T` is passed as value to
    /// that function. Formally,
    /// - If `T` is "array of `U`", then the decayed type is `U*`, such that
    ///   array bases are converted to pointers.
    /// - Otherwise, if `T` is a function type `F`, the decayed type is a
    ///   pointer to that function.
    /// - Otherwise, the decayed type becomes the one that removes the
    ///   cv-qualifier. For example, `const T *const var` becomes
    ///   `const T *param`.
    ///
    static Type decay(Type t1);

    Type decay();

    ///
    /// \brief Check if a implicit cast from `src` to `dst` is valid.
    ///        Warns if a lost of precision would occur.
    ///
    static bool implicitCast(Type dst, Type src);

    Ptr<const AST> declRoot; ///< The corresponding declaration, upon which
                             ///< the type system acts.

  protected:
    ///
    /// \brief Construct a type. The end user shall not use this constructor.
    /// Internally, `makeType` will copy the given declaration with necessary
    /// preprocessing, and then use the deep copy to construct this type.
    ///
    Type(Ptr<const AST> declRoot_);
};

// TODO: add:
//  - Type comparison
//  - Type promotion
//  - Implicit Cast/Explicit Cast

} // namespace splc

#endif // __SPLC_AST_TYPE_HH__