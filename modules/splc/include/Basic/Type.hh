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

#ifndef __SPLC_BASIC_TYPE_HH__
#define __SPLC_BASIC_TYPE_HH__ 1

#include "Core/Base.hh"
#include "Core/Utils/Logging.hh"
#include "Core/splc.hh"
#include <iterator>
#include <random>
#include <type_traits>
#include <vector>

namespace splc {

class SPLCContext;

class Type;
class PointerType;
class ArrayType;
class StructType;
class FunctionType;

template <class... Tys>
concept AreBaseOfType = (std::is_base_of_v<Type, Tys> && ...);

using TypePtrArray = std::vector<Type *>;

/// The instances of the Type class are immutable: once they are created,
/// they are never changed. Also note that only one instance of a particular
/// type is ever created. Thus seeing if two types are equal is a matter of
/// doing a trivial pointer comparison. To enforce that no two equal instances
/// are created, Type instances can only be created via static factory methods
/// in class Type and in derived classes. Once allocated, Types are never
/// free'd.
///
class Type {
  public:
    ///
    /// \brief TypeID of built-in primitive types.
    /// Definition of all of the base types for the Type system. Based on this
    /// value, you can cast to a class defined in DerivedTypes.hh
    ///
    enum class TypeID {
        // Primitive Types
        Void = 0,    ///< type with no size
        Float = 1,   ///< 32-bit floating point type
        Double = 2,  ///< 64-bit floating point type
        Int1 = 3,    ///< 1-bit integer
        UInt8 = 4,   ///< 8-bit unsigned integer
        SInt8 = 5,   ///< 8-bit signed integer
        UInt16 = 6,  ///< 16-bit unsigned integer
        SInt16 = 7,  ///< 16-bit signed integer
        UInt32 = 8,  ///< 32-bit unsigned integer
        SInt32 = 9,  ///< 32-bit signed integer
        UInt64 = 10, ///< 64-bit unsigned integer
        SInt64 = 11, ///< 64-bit signed integer
        Label = 12,  ///< Labels
        Token = 13,  ///< Tokens

        // Derived Types from DerivedTypes.hh
        Function = 16, ///< Functions
        Pointer = 17,  ///< Pointers
        Struct = 18,   ///< Structures
        Array = 19,    ///< Arrays
    };

  protected:
    friend class SPLCContext;
    explicit Type(SPLCContext &C, TypeID tid) : context(C), ID{tid} {}

    unsigned getSubclassData() const { return subClassData; }

    void setSubclassData(unsigned val)
    {
        subClassData = val;
        splc_assert(subClassData == val) << "subclass data too large for field";
    }

    unsigned numContainedTys = 0;
    Type *const *containedTys = nullptr;

  private:
    SPLCContext &context;
    TypeID ID;
    unsigned subClassData;
    // bool constTy;

  public:
    virtual ~Type() = default;

    std::string_view getName() const noexcept;

    friend std::ostream &operator<<(std::ostream &os, const Type &type)
    {
        os << "" << type.getName();
        if (type.numContainedTys > 0) {
            os << " (";
            for (auto it = type.subtype_begin(); it != type.subtype_end(); ++it) {
                os << **it;
                if (it + 1 != type.subtype_end())
                    os << ", ";
            }
            os << ")";
        }
        return os;
    }

    //===----------------------------------------------------------------------===//
    // Accessors
    SPLCContext &getContext() const { return context; }

    TypeID getTypeID() const { return ID; }

    bool isVoidTy() const { return getTypeID() == TypeID::Void; }

    bool isFloatTy() const { return getTypeID() == TypeID::Float; }

    bool isDoubleTy() const { return getTypeID() == TypeID::Double; }

    bool isFloatingPointTy() const { return isFloatTy() || isDoubleTy(); }

    bool isLabelTy() const { return getTypeID() == TypeID::Label; }

    bool isTokenTy() const { return getTypeID() == TypeID::Token; }

    bool isInt1Ty() const { return getTypeID() == TypeID::Int1; }

    bool isUInt8Ty() const { return getTypeID() == TypeID::UInt8; }

    bool isUInt16Ty() const { return getTypeID() == TypeID::UInt16; }

    bool isUInt32Ty() const { return getTypeID() == TypeID::UInt32; }

    bool isUInt64Ty() const { return getTypeID() == TypeID::UInt64; }

    bool isSInt8Ty() const { return getTypeID() == TypeID::SInt8; }

    bool isSInt16Ty() const { return getTypeID() == TypeID::SInt16; }

    bool isSInt32Ty() const { return getTypeID() == TypeID::SInt32; }

    bool isSInt64Ty() const { return getTypeID() == TypeID::SInt64; }

    bool isInt8Ty() const { return isUInt8Ty() || isSInt8Ty(); }

    bool isInt16Ty() const { return isUInt16Ty() || isSInt16Ty(); }

    bool isInt32Ty() const { return isUInt32Ty() || isSInt32Ty(); }

    bool isInt64Ty() const { return isUInt64Ty() || isSInt64Ty(); }

    bool isUIntTy() const
    {
        return isUInt8Ty() || isUInt16Ty() || isUInt32Ty() || isUInt64Ty();
    }

    bool isSIntTy() const
    {
        return isSInt8Ty() || isSInt16Ty() || isSInt32Ty() || isSInt64Ty();
    }

    // bool isConstTy() const { return constTy; }

    bool isIntTy() const { return isUIntTy() || isSIntTy() || isInt1Ty(); }

    bool isFunctionTy() const { return getTypeID() == TypeID::Function; }

    bool isPointerTy() const { return getTypeID() == TypeID::Pointer; }

    bool isIntOrPtrTy() const { return isIntTy() || isPointerTy(); }

    bool isStructTy() const { return getTypeID() == TypeID::Struct; }

    bool isArrayTy() const { return getTypeID() == TypeID::Array; }

    bool isEmptyTy() const;

    /// Return true if the type is "first class", meaning it is a valid type for
    /// a Value.
    bool isFirstClassType() const { return !isFunctionTy() && !isVoidTy(); }

    /// Return true if the type is a valid type for a register in codegen. This
    /// includes all first-class types except struct and array types.
    bool isSingleValueType() const
    {
        return isFloatingPointTy() || isIntTy() || isPointerTy();
    }

    bool isAggregateType() const { return isStructTy() || isArrayTy(); }

    /// Return true if it makes sense to take the size of this type.
    bool isSized() const
    {
        if (isIntTy() || isFloatTy() || isPointerTy())
            return true;

        if (!isStructTy() && !isArrayTy())
            return false;

        return isSizedDerivedType();
    }

    size_t getPrimitiveSizeInBits() const;

    //===----------------------------------------------------------------------===//
    // Type Iteration support.
    using subtype_iterator = Type *const *;
    subtype_iterator subtype_begin() const { return containedTys; }
    subtype_iterator subtype_end() const
    {
        return &containedTys[numContainedTys];
    }
    TypePtrArray subtypes() const { return {subtype_begin(), subtype_end()}; }

    using subtype_reverse_iterator = std::reverse_iterator<subtype_iterator>;

    subtype_reverse_iterator subtype_rbegin() const
    {
        return subtype_reverse_iterator(subtype_end());
    }

    subtype_reverse_iterator subtype_rend() const
    {
        return subtype_reverse_iterator(subtype_begin());
    }

    Type *getContainedType(unsigned i) const
    {
        splc_assert(i < numContainedTys)
            << "index out of range: " << i << " of " << numContainedTys;
        return containedTys[i];
    }

    unsigned getIntegerBitWidth() const;

    Type *getFunctionParamType(unsigned i) const;

    unsigned getFunctionNumParams() const;

    bool isFunctionVarArg() const;

    std::string_view getStructName() const;
    unsigned getStructNumElements() const;
    Type *getStructElementType(unsigned i) const;

    uint64_t getArrayNumElements() const;

    Type *getArrayElementType() const
    {
        splc_assert(isArrayTy())
            << "getArrayElementType() called on non-array type.";
        return containedTys[0];
    }

    static Type *getPrimitiveType(SPLCContext &C, TypeID ID);

    static Type *getFloatTy(SPLCContext &C);
    static Type *getDoubleTy(SPLCContext &C);
    static Type *getVoidTy(SPLCContext &C);
    static Type *getLabelTy(SPLCContext &C);
    static Type *getTokenTy(SPLCContext &C);
    static Type *getInt1Ty(SPLCContext &C);
    static Type *getUInt8Ty(SPLCContext &C);
    static Type *getSInt8Ty(SPLCContext &C);
    static Type *getUInt16Ty(SPLCContext &C);
    static Type *getSInt16Ty(SPLCContext &C);
    static Type *getUInt32Ty(SPLCContext &C);
    static Type *getSInt32Ty(SPLCContext &C);
    static Type *getUInt64Ty(SPLCContext &C);
    static Type *getSInt64Ty(SPLCContext &C);

    /// Return a signed type of the current type.
    Type *getSigned() const;

    /// Return an unsigned type of the current type.
    Type *getUnsigned() const;

    /// Return a pointer to the current type.
    PointerType *getPointerTo() const;

    /// Return a const version of the current type.
    // Type *getConstTy() const;

  private:
    bool isSizedDerivedType() const;
};

using TypeID = Type::TypeID;

/// Reference: https://stackoverflow.com/a/24586587
inline std::string randomTypeName(std::string::size_type length)
{
    static std::string chrs{"0123456789"
                            "abcdefghijklmnopqrstuvwxyz"
                            "ABCDEFGHIJKLMNOPQRSTUVWXYZ"};

    thread_local static std::mt19937 rg{std::random_device{}()};
    thread_local static std::uniform_int_distribution<std::string::size_type>
        pick(0, std::size(chrs) - 2);

    std::string s;

    s.reserve(length);

    while (length--)
        s += chrs[pick(rg)];

    return s;
}

// TODO: add:
//  - Type comparison
//  - Type promotion
//  - Implicit Cast/Explicit Cast

} // namespace splc

#endif // __SPLC_BASIC_TYPE_HH__