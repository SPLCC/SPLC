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

#ifndef __SPLC_AST_TYPE_CONTEXT_HH__
#define __SPLC_AST_TYPE_CONTEXT_HH__ 1

#include "Core/Base.hh"
#include "Core/splc.hh"
#include "Type.hh"
#include <map>
#include <set>

namespace splc {

// using TypeMap = std::map<>;

/// Signature: return value, vector of arguments, bool
using FunctionTypeKeyInfo = std::tuple<Type *, TypePtrArray, bool>;
using AnonStructTypeKeyInfo = std::tuple<TypePtrArray>;
using ArrayKeyInfo = std::tuple<Type *, uint64_t>;
using PointerKeyInfo = Type *;

// TODO: maybe switch to threaded context
class TypeContext {
  public:
    TypeContext()
        : VoidTy{*this, TypeID::Void}, FloatTy{*this, TypeID::Float},
          DoubleTy{*this, TypeID::Double}, Int1Ty{*this, TypeID::Int1},
          UInt8Ty{*this, TypeID::UInt8}, SInt8Ty{*this, TypeID::SInt8},
          UInt16Ty{*this, TypeID::UInt16}, SInt16Ty{*this, TypeID::SInt16},
          UInt32Ty{*this, TypeID::UInt32}, SInt32Ty{*this, TypeID::SInt32},
          UInt64Ty{*this, TypeID::UInt64}, SInt64Ty{*this, TypeID::SInt64},
          LabelTy{*this, TypeID::Label}, TokenTy{*this, TypeID::Token}
    {
    }

    Type VoidTy, FloatTy, DoubleTy, Int1Ty, UInt8Ty, SInt8Ty, UInt16Ty,
        SInt16Ty, UInt32Ty, SInt32Ty, UInt64Ty, SInt64Ty, LabelTy, TokenTy;

    std::map<FunctionTypeKeyInfo, FunctionType *, std::less<>>
        functionTypes; ///< function types

    std::map<AnonStructTypeKeyInfo, StructType *, std::less<>>
        anonStructTypes; ///< Anonymous structures

    std::map<const std::string, StructType *, std::less<>>
        namedStructTypes; ///< Anonymous structures

    std::map<ArrayKeyInfo, ArrayType *> arrayTypes;

    std::map<PointerKeyInfo, PointerType *> pointerTypes;
};

} // namespace splc

#endif // __SPLC_AST_TYPE_CONTEXT_HH__