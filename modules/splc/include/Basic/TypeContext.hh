//===- splc/TypeContext.hh - Classes for storing types ----------*- C++ -*-===//
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
#ifndef __SPLC_BASIC_TYPE_CONTEXT_HH__
#define __SPLC_BASIC_TYPE_CONTEXT_HH__ 1

#include "Core/splc.hh"
#include "Type.hh"
#include <map>
#include <memory>

namespace splc {

// using TypeMap = std::map<>;

/// Signature: return value, vector of arguments, bool
using FunctionTypeKeyInfo = std::tuple<Type *, TypePtrArray, bool>;
using AnonStructTypeKeyInfo = std::tuple<TypePtrArray>;
using ArrayKeyInfo = std::tuple<Type *, uint64_t>;
using PointerKeyInfo = Type *;

class TypeContext {
  public:
    TypeContext() noexcept
        : VoidTy{*this, TypeID::Void}, FloatTy{*this, TypeID::Float},
          DoubleTy{*this, TypeID::Double}, Int1Ty{*this, TypeID::Int1},
          UInt8Ty{*this, TypeID::UInt8}, SInt8Ty{*this, TypeID::SInt8},
          UInt16Ty{*this, TypeID::UInt16}, SInt16Ty{*this, TypeID::SInt16},
          UInt32Ty{*this, TypeID::UInt32}, SInt32Ty{*this, TypeID::SInt32},
          UInt64Ty{*this, TypeID::UInt64}, SInt64Ty{*this, TypeID::SInt64},
          LabelTy{*this, TypeID::Label}, TokenTy{*this, TypeID::Token}
    {
    }

    TypeContext(const TypeContext &) = delete;
    TypeContext &operator=(const TypeContext &) = delete;

    ~TypeContext() noexcept
    {
        for (auto &ent : tyAllocTraceMap) {
            tyAllocator.deallocate(reinterpret_cast<char *>(ent.first),
                                   ent.second);
        }
        tyAllocTraceMap.clear();
    }

    Type VoidTy, FloatTy, DoubleTy, Int1Ty, UInt8Ty, SInt8Ty, UInt16Ty,
        SInt16Ty, UInt32Ty, SInt32Ty, UInt64Ty, SInt64Ty, LabelTy, TokenTy;

    std::map<FunctionTypeKeyInfo, FunctionType *, std::less<>>
        functionTypes; ///< function types

    std::map<AnonStructTypeKeyInfo, StructType *, std::less<>>
        anonStructTypes; ///< Anonymous structures

    std::map<const std::string, StructType *, std::less<>>
        namedStructTypes; ///< Named structures

    std::map<ArrayKeyInfo, ArrayType *> arrayTypes;

    std::map<PointerKeyInfo, PointerType *> pointerTypes;

    /// Allocate for future use.
    template <class T>
    T *tyAlloc(size_t n = 1)
    {
        size_t tySz = sizeof(T);
        size_t totalSz = n * tySz;
        void *p = tyAllocator.allocate(totalSz);
        tyAllocTraceMap.insert({p, totalSz});
        return reinterpret_cast<T *>(p);
    }

    /// Deallocate
    template <class T>
    void tyDealloc(T *ptr, size_t n = 1)
    {
        auto it = tyAllocTraceMap.find(ptr);
        splc_assert(it != tyAllocTraceMap.end() && it->second == n) << "invalid release";
        tyAllocator.deallocate(reinterpret_cast<char *>(ptr), n);
        tyAllocTraceMap.erase(it);
    }

    std::allocator<char> tyAllocator;
    std::map<void *, size_t> tyAllocTraceMap;
};

} // namespace splc

#endif // __SPLC_BASIC_TYPE_CONTEXT_HH__