//===- Type.cpp - Implement the Type class --------------------------------===//
//
// Part of the SPLC project. Reference from the LLVM project under the Apache
// License v2.0 with LLVM exceptions.
// This file is a selected subset with manual modifications to satisfy
// requirements for course SUSTech CS323-Compiler.
//
//===----------------------------------------------------------------------===//
//
// This file implements the Type class for SPLC.
//
//===----------------------------------------------------------------------===//

#include "Basic/Type.hh"
#include "Basic/SPLCContext.hh"
#include "Core/Base.hh"

#include "AST/DerivedAST.hh"
#include "Basic/DerivedTypes.hh"
#include "Core/Utils/Logging.hh"
#include <algorithm>
#include <utility>

namespace splc {

//===----------------------------------------------------------------------===//
//                       Type Class Implementation
//===----------------------------------------------------------------------===//

const char *typeNames[] = {"void",
                           "32-bit floating-point",
                           "64-bit floating-point",
                           "1-bit int",
                           "8-bit unsigned int",
                           "8-bit signed int",
                           "16-bit unsigned int",
                           "16-bit signed int",
                           "32-bit unsigned int",
                           "32-bit signed int",
                           "64-bit unsigned int",
                           "64-bit signed int",
                           "label",
                           "token",
                           nullptr,
                           nullptr,
                           "function",
                           "pointer",
                           "structure",
                           "array"};

std::string_view Type::getName() const noexcept
{
    return {typeNames[static_cast<int>(ID)]};
}

Type *Type::getPrimitiveType(SPLCContext &C, TypeID ID)
{
    switch (ID) {
    case TypeID::Void:
        return getVoidTy(C);
    case TypeID::Float:
        return getFloatTy(C);
    case TypeID::Double:
        return getDoubleTy(C);
    case TypeID::Int1:
        return getInt1Ty(C);
    case TypeID::UInt16:
        return getUInt16Ty(C);
    case TypeID::SInt16:
        return getSInt16Ty(C);
    case TypeID::UInt32:
        return getUInt32Ty(C);
    case TypeID::SInt32:
        return getSInt32Ty(C);
    case TypeID::UInt64:
        return getUInt64Ty(C);
    case TypeID::SInt64:
        return getSInt64Ty(C);
    case TypeID::Label:
        return getLabelTy(C);
    case TypeID::Token:
        return getTokenTy(C);
    default:
        return nullptr;
    }
}

bool Type::isSizedDerivedType() const
{
    if (this->isArrayTy()) {
        auto *aty = dynamic_cast<const ArrayType *>(this);
        return aty->getElementType()->isSized();
    }
    if (this->isStructTy()) {
        auto *sty = dynamic_cast<const StructType *>(this);
        return sty->isSized();
    }
    return false;
}

Type *Type::getSigned() const
{
    if(!isIntTy())
        return nullptr;
    if (isSIntTy())
        return const_cast<Type *>(this);

    switch (ID) {
    case TypeID::UInt8:
        return getSInt8Ty(context);
    case TypeID::UInt16:
        return getSInt16Ty(context);
    case TypeID::UInt32:
        return getSInt32Ty(context);
    case TypeID::UInt64:
        return getSInt64Ty(context);
    default:
        return nullptr;
    }
}

Type *Type::getUnsigned() const
{
    if(!isIntTy())
        return nullptr;
    if (isUIntTy())
        return const_cast<Type *>(this);

    switch (ID) {
    case TypeID::SInt8:
        return getUInt8Ty(context);
    case TypeID::SInt16:
        return getUInt16Ty(context);
    case TypeID::SInt32:
        return getUInt32Ty(context);
    case TypeID::SInt64:
        return getUInt64Ty(context);
    default:
        return nullptr;
    }
}

//===----------------------------------------------------------------------===//
//                       Primitive Type Data
//===----------------------------------------------------------------------===//
size_t Type::getPrimitiveSizeInBits() const
{
    switch (getTypeID()) {
    case TypeID::Void:
        return 0;
    case TypeID::Float:
        return 32;
    case TypeID::Double:
        return 64;
    case TypeID::Int1:
        return 1;
    case TypeID::UInt8:
    case TypeID::SInt8:
        return 8;
    case TypeID::UInt16:
    case TypeID::SInt16:
        return 16;
    case TypeID::UInt32:
    case TypeID::SInt32:
        return 32;
    case TypeID::UInt64:
    case TypeID::SInt64:
        return 64;
    case TypeID::Label:
    case TypeID::Token:
    case TypeID::Pointer:
        // assume 32-bit address space
        return 32;
    default:
        return 0;
    }
}

Type *Type::getVoidTy(SPLCContext &C) { return &C.VoidTy; }
Type *Type::getFloatTy(SPLCContext &C) { return &C.FloatTy; }
Type *Type::getDoubleTy(SPLCContext &C) { return &C.DoubleTy; }
Type *Type::getInt1Ty(SPLCContext &C) { return &C.Int1Ty; }
Type *Type::getUInt8Ty(SPLCContext &C) { return &C.UInt8Ty; }
Type *Type::getSInt8Ty(SPLCContext &C) { return &C.SInt8Ty; }
Type *Type::getUInt16Ty(SPLCContext &C) { return &C.UInt16Ty; }
Type *Type::getSInt16Ty(SPLCContext &C) { return &C.SInt16Ty; }
Type *Type::getUInt32Ty(SPLCContext &C) { return &C.UInt32Ty; }
Type *Type::getSInt32Ty(SPLCContext &C) { return &C.SInt32Ty; }
Type *Type::getUInt64Ty(SPLCContext &C) { return &C.UInt64Ty; }
Type *Type::getSInt64Ty(SPLCContext &C) { return &C.SInt64Ty; }
Type *Type::getLabelTy(SPLCContext &C) { return &C.LabelTy; }
Type *Type::getTokenTy(SPLCContext &C) { return &C.TokenTy; }

//===----------------------------------------------------------------------===//
//                       FunctionType Implementation
//===----------------------------------------------------------------------===//
FunctionType::FunctionType(Type *retTy, const std::vector<Type *> &params,
                           bool isVarArg)
    : Type{retTy->getContext(), TypeID::Function}
{
    splc_assert(isValidReturnType(retTy)) << "invalid return type for function";
    setSubclassData(isVarArg);
    Type **subTys = new (getContext().tyAlloc<Type *>(params.size() + 1))
        Type *[params.size() + 1];
    subTys[0] = retTy;
    for (unsigned i = 0, e = params.size(); i != e; ++i) {
        splc_assert(isValidArgumentType(params[i]))
            << "invalid type for function argument";
        subTys[i + 1] = params[i];
    }
    containedTys = subTys;
    numContainedTys = params.size() + 1;
}

FunctionType *FunctionType::get(Type *retTy, const std::vector<Type *> &params,
                                bool isVarArg)
{
    auto &context = retTy->getContext();
    const FunctionTypeKeyInfo key{retTy, params, isVarArg};
    FunctionType *ft;

    // Just perform two lookups, whatsoever
    auto &funcMap = context.functionTypes;
    auto it = funcMap.find(key);
    if (it == funcMap.end()) {
        // Not found
        ft = new (context.tyAlloc<FunctionType>())
            FunctionType(retTy, params, isVarArg);
        funcMap.insert(std::make_pair(key, ft));
    }
    else {
        ft = it->second;
    }
    return ft;
}

FunctionType *FunctionType::get(Type *retTy, bool isVarArg)
{
    return get(retTy, {}, isVarArg);
}

bool FunctionType::isValidReturnType(Type *retTy)
{
    return !retTy->isFunctionTy() && !retTy->isLabelTy();
}

bool FunctionType::isValidArgumentType(Type *argTy)
{
    return argTy->isFirstClassType();
}

//===----------------------------------------------------------------------===//
//                          StructType Implementation
//===----------------------------------------------------------------------===//

StructType *StructType::get(SPLCContext &context, TypePtrArray elements)
{
    const AnonStructTypeKeyInfo key{elements};
    StructType *st;

    // Just perform two lookups, whatsoever
    auto &structMap = context.anonStructTypes;
    auto it = structMap.find(key);
    if (it == structMap.end()) {
        // Not found
        st = new (context.tyAlloc<StructType>()) StructType(context);
        st->setSubclassData(SCDB_IsLiteral);
        st->setBody(elements);
        structMap.insert(std::make_pair(key, st));
    }
    else {
        st = it->second;
    }
    return st;
}

void StructType::setBody(TypePtrArray elements)
{
    splc_assert(isOpaque()) << "struct body has already been set.";
    setSubclassData((getSubclassData()) | SCDB_HasBody);

    numContainedTys = elements.size();
    if (elements.empty()) {
        containedTys = nullptr;
        return;
    }

    Type **subTys =
        new (elements[0]->getContext().tyAlloc<Type *>(elements.size()))
            Type *[elements.size()];
    std::copy(elements.begin(), elements.end(), subTys);
    containedTys = subTys;
}

void StructType::setName(std::string_view name)
{
    if (name == getName())
        return;

    auto &map = getContext().namedStructTypes;

    // If this struct has a name, try remove its name
    auto it = map.find(getName());
    if (it != map.end()) {
        map.erase(it);
    }

    // If this is just removing the name, we're done
    // cool!
    if (name.empty()) {
        this->name = "";
        return;
    }

    // No name collision is allowed!
    splc_assert((it = map.find(name)) != map.end())
        << "duplicated structure name is not allowed in type system";
    map.insert(std::make_pair(std::string{name}, this));
}

std::string_view StructType::getName() const noexcept
{
    if (!isLiteral())
        return "unnamed struct";
    return name;
}

//===----------------------------------------------------------------------===//
//                         StructType helper functions

StructType *StructType::create(SPLCContext &context, std::string_view name)
{
    StructType *st = new (context.tyAlloc<StructType>()) StructType(context);
    if (!name.empty())
        st->setName(name);
    return st;
}

StructType *StructType::create(SPLCContext &context)
{
    return create(context, "");
}

StructType *StructType::create(const TypePtrArray &elements,
                               std::string_view name)
{
    return create(elements[0]->getContext(), elements, name);
}

StructType *StructType::create(const TypePtrArray &elements)
{
    return create(elements[0]->getContext(), elements);
}

StructType *StructType::create(SPLCContext &context,
                               const TypePtrArray &elements,
                               std::string_view name)
{
    StructType *st = create(context, name);
    st->setBody(elements);
    return st;
}

StructType *StructType::create(SPLCContext &context,
                               const TypePtrArray &elements)
{
    return create(context, elements, "");
}

/// Create an empty structure type.
StructType *StructType::get(SPLCContext &context) { return get(context, {}); }

bool StructType::isSized() const
{
    if ((getSubclassData() & SCDB_IsSized) != 0)
        return true;
    if (isOpaque())
        return false;

    // Check if all elements are sized
    for (Type *ty : elements()) {
        if (!ty->isSized()) {
            return false;
        }
    }

    // Cheat a bit to apply this unidirectional change: only opaque -> sized is
    // allowed.
    const_cast<StructType *>(this)->setSubclassData(getSubclassData() |
                                                    SCDB_IsSized);
    return true;
}

bool StructType::isLayoutIdentical(StructType *other) const
{
    return elements() ==
           other->elements(); // compare vectors of their respective elements
}

//===----------------------------------------------------------------------===//
//                           ArrayType Implementation
//===----------------------------------------------------------------------===//

ArrayType::ArrayType(Type *eltTy, uint64_t numElts)
    : Type{eltTy->getContext(), TypeID::Array}, elementType{eltTy},
      numElements(numElts)
{
    containedTys = &elementType;
    numContainedTys = 1;
}

ArrayType *ArrayType::get(Type *elementType_, uint64_t numElements_)
{
    splc_assert(isValidElementType(elementType_))
        << "Invalid type for array element!";

    ArrayType *at = nullptr;

    SPLCContext &context = elementType_->getContext();
    auto &arraySet = context.arrayTypes;
    ArrayKeyInfo key{elementType_, numElements_};

    auto it = arraySet.find(key);
    if (it == arraySet.end()) {
        // Not found
        at = new (context.tyAlloc<ArrayType>())
            ArrayType(elementType_, numElements_);
        arraySet.insert({key, at});
        return at;
    }

    return it->second;
}

bool ArrayType::isValidElementType(Type *ElemTy)
{
    return !ElemTy->isVoidTy() && !ElemTy->isLabelTy() &&
           !ElemTy->isFunctionTy() && !ElemTy->isTokenTy();
}

//===----------------------------------------------------------------------===//
//                         PointerType Implementation
//===----------------------------------------------------------------------===//

PointerType *PointerType::get(Type *elementType)
{
    splc_assert(elementType) << "cannot get a pointer to nullptr";
    splc_assert(isValidElementType(elementType))
        << "invalid type for pointer element";

    PointerType *pt = nullptr;
    auto &context = elementType->getContext();
    PointerKeyInfo key{elementType};
    auto &map = context.pointerTypes;
    auto it = map.find(key);

    if (it == map.end()) {
        // Not found
        pt = new (context.tyAlloc<PointerType>())
            PointerType(context, elementType);
        map.insert(std::make_pair(key, pt));
        return pt;
    }

    pt = it->second;
    return pt;
}

PointerType *PointerType::get(SPLCContext &C)
{
    PointerType *pt = nullptr;
    PointerKeyInfo key{&C.VoidTy};
    auto &map = C.pointerTypes;
    auto it = map.find(key);

    if (it == map.end()) {
        // Not found
        pt = new (C.tyAlloc<PointerType>()) PointerType(C, &C.VoidTy);
        map.insert(std::make_pair(key, pt));
        return pt;
    }

    pt = it->second;
    return pt;
}

PointerType::PointerType(SPLCContext &C, Type *elementType_)
    : Type{C, TypeID::Pointer}, elementType{elementType_}
{
    numContainedTys = 1;
    containedTys = &elementType;
}

PointerType *Type::getPointerTo() const
{
    return PointerType::get(const_cast<Type *>(this));
}

// Type *Type::getConstTy() const
// {

// }

bool PointerType::isValidElementType(Type *elementType_)
{
    return !elementType_->isVoidTy() && !elementType_->isLabelTy() &&
           !elementType_->isTokenTy();
}

bool PointerType::isLoadableOrStorableType(Type *elementType)
{
    return isValidElementType(elementType) && !elementType->isFunctionTy();
}

} // namespace splc