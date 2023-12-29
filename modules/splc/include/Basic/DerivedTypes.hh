//===- splc/Type.hh - Classes for handling data types -----------*- C++ -*-===//
//
// Part of the SPLC project. Reference from the LLVM project under the Apache
// License v2.0 with LLVM exceptions.
// This file is a selected subset with manual modifications to satisfy
// requirements for course SUSTech CS323-Compiler.
//
//===----------------------------------------------------------------------===//
//
// This file contains the declaration of the Type class.  For more "Type"
// stuff, look in DerivedTypes.h.
//
//===----------------------------------------------------------------------===//
#ifndef __SPLC_BASIC_DERIVED_TYPES_HH__
#define __SPLC_BASIC_DERIVED_TYPES_HH__ 1

#include "AST/ASTCommons.hh"
#include "Core/splc.hh"
#include <Basic/Type.hh>
#include <iterator>
#include <vector>

namespace splc {

class PointerType;
class TypeContext;

class FunctionType : public Type {
    FunctionType(Type *retTy, const TypePtrArray &params, bool isVarArg);

  public:
    FunctionType(const FunctionType &) = delete;
    FunctionType &operator=(const FunctionType &) = delete;

    /// This static method is the primary way of constructing a FunctionType.
    static FunctionType *get(Type *result, const TypePtrArray &params,
                             bool isVarArg);

    /// Create a FunctionType taking no parameters.
    static FunctionType *get(Type *result, bool isVarArg);

    /// Return true if the specified type is valid as a return type.
    static bool isValidReturnType(Type *retTy);

    /// Return true if the specified type is valid as an argument type.
    static bool isValidArgumentType(Type *argTy);

    bool isVarArg() const { return getSubclassData() != 0; }

    Type *getReturnType() const { return containedTys[0]; }

    using param_iterator = Type::subtype_iterator;

    param_iterator param_begin() const { return containedTys + 1; }
    param_iterator param_end() const { return &containedTys[numContainedTys]; }
    TypePtrArray params() const { return {param_begin(), param_end()}; }

    Type *getParamType(unsigned i) const
    {
        splc_assert(i < getNumParams())
            << "getParamType out of range: " << i << " of " << getNumParams();
        return containedTys[i + 1];
    }

    unsigned getNumParams() const { return numContainedTys - 1; }

    // Finally, just do a direct cast
};

inline bool Type::isFunctionVarArg() const
{
    return dynamic_cast<const FunctionType *>(this)->isVarArg();
}

inline Type *Type::getFunctionParamType(unsigned i) const
{
    return dynamic_cast<const FunctionType *>(this)->getParamType(i);
}

inline unsigned Type::getFunctionNumParams() const
{
    return dynamic_cast<const FunctionType *>(this)->getNumParams();
}

class StructType : public Type {
    friend class TypeContext;

    StructType(TypeContext &C) : Type{C, TypeID::Struct} {}

    enum {
        SCDB_HasBody = 1,   ///< If this type has body
        SCDB_IsLiteral = 2, ///< If this type is uniqued
                            ///< by structural equivalence,
        SCDB_IsSized = 4,   ///< Return true if this is a sized type.
    };

    std::string_view name;

  public:
    StructType(const StructType &) = delete;
    StructType &operator=(const StructType &) = delete;

    static StructType *create(TypeContext &context, std::string_view name);
    static StructType *create(TypeContext &context);
    static StructType *create(const TypePtrArray &elements,
                              std::string_view name);
    static StructType *create(const TypePtrArray &elements);
    static StructType *create(TypeContext &context,
                              const TypePtrArray &elements,
                              std::string_view name);
    static StructType *create(TypeContext &context,
                              const TypePtrArray &elements);

    template <AreBaseOfType... Tys>
    static StructType *create(std::string_view name, Type *elt1, Tys *...elts)
    {
        splc_assert(elt1)
            << "there must be at least one element inside a struct";
        return create(TypePtrArray{{elt1, elts...}}, name);
    }

    /// This static method is the primary way to create a literal StructType.
    static StructType *get(TypeContext &context, TypePtrArray elements);

    /// Create an empty structure type.
    static StructType *get(TypeContext &context);

    /// This static method is a convenience method for creating structure types
    /// by specifying the elements as arguments. Note that this method always
    /// returns a non-packed struct, and requires at least one element type.
    template <AreBaseOfType... Tys>
    static StructType *get(Type *elt1, Tys *...elts)
    {
        splc_assert(elt1)
            << "there must be at least one element inside a struct";
        auto &ctx = elt1->getContext();
        return StructType::get(ctx, TypePtrArray{{elt1, elts...}});
    }

    static StructType *getTypeByName(TypeContext &C, std::string_view name);

    /// Return true if this type is uniqued by structural equivalence, false if
    /// it is a struct definition.
    bool isLiteral() const { return (getSubclassData() & SCDB_IsLiteral) != 0; }

    /// Return true if this is a type with an identity that has no body
    /// specified yet.
    bool isOpaque() const { return (getSubclassData() & SCDB_HasBody) != 0; }

    /// isSized - Return true if this is a sized type.
    bool isSized() const;

    static bool isValidElementType(Type *elemTy);
    bool hasName() const { return !name.empty(); }

    void setName(std::string_view name);

    std::string_view getName() const noexcept;

    void setBody(TypePtrArray elements);

    template <AreBaseOfType... Tys>
    void setBody(Type *elt1, Tys *...elts)
    {
        splc_assert(elt1)
            << "cannot create a struct type with no elements within";
        setBody(TypePtrArray{{elt1, elts...}});
    }

    // Iterator access to the elements.
    using element_iterator = Type::subtype_iterator;

    element_iterator element_begin() const { return containedTys; }
    element_iterator element_end() const
    {
        return &containedTys[numContainedTys];
    }
    TypePtrArray elements() const { return {element_begin(), element_end()}; }

    /// Return true if this is layout identical to the specified struct.
    bool isLayoutIdentical(StructType *other) const;

    unsigned getNumElements() const { return numContainedTys; }

    Type *getElementType(unsigned i) const
    {
        splc_assert(i < numContainedTys) << "getElementType out of range: " << i
                                         << " of " << numContainedTys;
        return containedTys[i];
    }

    Type *getTypeAtIndex(unsigned i) const { return getElementType(i); }

    bool indexValid(unsigned idx) const { return idx < getNumElements(); }
};

inline unsigned Type::getStructNumElements() const
{
    return dynamic_cast<const StructType *>(this)->getNumElements();
}

inline Type *Type::getStructElementType(unsigned i) const
{
    return dynamic_cast<const StructType *>(this)->getElementType(i);
}

class ArrayType : public Type {
    Type *elementType;

    uint64_t numElements;

    ArrayType(Type *eltTy, uint64_t numElts);

  public:
    ArrayType(const ArrayType &) = delete;
    ArrayType &operator=(const ArrayType &) = delete;

    uint64_t getNumElements() const { return numElements; }

    Type *getElementType() const { return elementType; }

    static ArrayType *get(Type *elementType_, uint64_t numElements_);

    static bool isValidElementType(Type *elementType);
};

inline uint64_t Type::getArrayNumElements() const
{
    return dynamic_cast<const ArrayType *>(this)->getNumElements();
}

class PointerType : public Type {
    Type *elementType;

    explicit PointerType(TypeContext &C, Type *elementType_);

  public:
    PointerType(const PointerType &) = delete;
    PointerType &operator=(const PointerType &) = delete;

    /// This constructs a pointer to an object of the specified type.
    static PointerType *get(Type *elementType);

    /// This constructs a void pointer to an object.
    static PointerType *get(TypeContext &C);

    /// Return true if the specified type is valid as a element type.
    static bool isValidElementType(Type *elementType_);

    /// Return true if we can load or store from a pointer to this type.
    static bool isLoadableOrStorableType(Type *elementType);
};

} // namespace splc

#endif // __SPLC_BASIC_DERIVED_TYPES_HH__