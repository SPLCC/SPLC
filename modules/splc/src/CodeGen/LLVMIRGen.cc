#include "CodeGen/LLVMIRGen.hh"

namespace splc {

llvm::Type *LLVMIRBuilder::getPrimitiveType(splc::Type *ty)
{
    auto &cachedTy = tyCache[ty];

    if (cachedTy != nullptr) {
        return cachedTy;
    }

    switch (ty->getTypeID()) {
    case TypeID::Void:
        return cachedTy = llvm::Type::getVoidTy(getLLVMCtx());
    case TypeID::Float:
        return cachedTy = llvm::Type::getFloatTy(getLLVMCtx());
    case TypeID::Double:
        return cachedTy = llvm::Type::getDoubleTy(getLLVMCtx());
    case TypeID::Int1:
        return cachedTy = llvm::Type::getInt1Ty(getLLVMCtx());
    case TypeID::UInt16:
    case TypeID::SInt16:
        return cachedTy = llvm::Type::getInt16Ty(getLLVMCtx());
    case TypeID::UInt32:
    case TypeID::SInt32:
        return cachedTy = llvm::Type::getInt32Ty(getLLVMCtx());
    case TypeID::UInt64:
    case TypeID::SInt64:
        return cachedTy = llvm::Type::getInt64Ty(getLLVMCtx());
    case TypeID::Label:
        return cachedTy = llvm::Type::getLabelTy(getLLVMCtx());
    case TypeID::Token:
        return cachedTy = llvm::Type::getTokenTy(getLLVMCtx());
    default:
        splc_ilog_error(nullptr, false) << "requested invalid type " << *ty;
        return nullptr;
    }
}

llvm::FunctionType *LLVMIRBuilder::getFunctionType(splc::FunctionType *ty)
{
    splc_dbgassert(ty->isFunctionTy());
    auto &cachedTy = tyCache[ty];
    if (cachedTy != nullptr)
        return static_cast<llvm::FunctionType *>(cachedTy);

    llvm::Type *resRetTy;
    std::vector<llvm::Type *> resArgTys;

    auto splcSubTys = ty->subtypes();
    resRetTy = getCvtType(ty->getReturnType());
    for (auto i = 1; i < splcSubTys.size(); ++i) {
        resArgTys.push_back(getCvtType(splcSubTys[i]));
    }

    cachedTy = llvm::FunctionType::get(resRetTy, resArgTys, false);
    return static_cast<llvm::FunctionType *>(cachedTy);
}

llvm::StructType *LLVMIRBuilder::getStructType(splc::StructType *ty)
{
    splc_dbgassert(ty->isStructTy());

    auto &cachedTy = tyCache[ty];
    if (cachedTy != nullptr)
        return static_cast<llvm::StructType *>(cachedTy);

    std::vector<llvm::Type *> resMemberTys;

    for (auto ty0 : ty->subtypes()) {
        resMemberTys.push_back(getCvtType(ty0));
    }

    cachedTy = llvm::StructType::get(getLLVMCtx(), resMemberTys);
    return static_cast<llvm::StructType *>(cachedTy);
}

llvm::PointerType *LLVMIRBuilder::getPointerType(splc::PointerType *ty)
{
    splc_dbgassert(ty->isPointerTy());

    auto &cachedTy = tyCache[ty];
    if (cachedTy != nullptr)
        return static_cast<llvm::PointerType *>(cachedTy);

    auto containedTy = tyCache[ty->getContainedType(0)];
    if (containedTy == nullptr) {
        containedTy = getCvtType(ty->getContainedType(0));
    }

    cachedTy = llvm::PointerType::get(containedTy, defaultAddrSpace);
    return static_cast<llvm::PointerType *>(cachedTy);
}

llvm::ArrayType *LLVMIRBuilder::getArrayType(splc::ArrayType *ty)
{
    splc_dbgassert(ty->isArrayTy());

    auto &cachedTy = tyCache[ty];
    if (cachedTy != nullptr)
        return static_cast<llvm::ArrayType *>(cachedTy);

    auto containedTy = tyCache[ty->getArrayElementType()];
    if (containedTy == nullptr) {
        containedTy = getCvtType(ty->getArrayElementType());
    }
    cachedTy = llvm::ArrayType::get(containedTy, ty->getArrayNumElements());
    return static_cast<llvm::ArrayType *>(cachedTy);
}

llvm::Type *LLVMIRBuilder::getCvtType(splc::Type *ty)
{
    auto extTy = tyCache[ty];
    if (extTy != nullptr) {
        return extTy;
    }

    switch (ty->getTypeID()) {
    case Type::TypeID::Function:
        return getFunctionType(dynamic_cast<splc::FunctionType *>(ty));
    case Type::TypeID::Struct:
        return getStructType(dynamic_cast<splc::StructType *>(ty));
    case Type::TypeID::Pointer:
        return getPointerType(dynamic_cast<splc::PointerType *>(ty));
    case Type::TypeID::Array:
        return getArrayType(dynamic_cast<splc::ArrayType *>(ty));
    default:
        return getPrimitiveType(ty);
    }
}

} // namespace splc