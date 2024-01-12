#include "CodeGen/ObjBuilder.hh"
#include <ranges>

namespace splc {

//===----------------------------------------------------------------------===//
//                          LLVMIRBuilder Implementation
//===----------------------------------------------------------------------===//

std::atomic<int> ObjBuilder::moduleCnt = 0;

ObjBuilder::~ObjBuilder() = default;

//===----------------------------------------------------------------------===//
//                               Type Conversions
//===----------------------------------------------------------------------===//

llvm::Type *ObjBuilder::getPrimitiveType(splc::Type *ty)
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

llvm::FunctionType *ObjBuilder::getFunctionType(splc::FunctionType *ty)
{
    splc_dbgassert(ty->isFunctionTy());
    auto &cachedTy = tyCache[ty];
    if (cachedTy != nullptr)
        return static_cast<llvm::FunctionType *>(cachedTy);

    llvm::Type *resRetTy;
    std::vector<llvm::Type *> resArgTys;

    auto splcSubTys = ty->subtypes();
    resRetTy = getCvtType(ty->getReturnType());
    for (unsigned i = 1; i < splcSubTys.size(); ++i) {
        resArgTys.push_back(getCvtType(splcSubTys[i]));
    }

    cachedTy = llvm::FunctionType::get(resRetTy, resArgTys, false);
    return static_cast<llvm::FunctionType *>(cachedTy);
}

llvm::StructType *ObjBuilder::getStructType(splc::StructType *ty)
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

llvm::PointerType *ObjBuilder::getPointerType(splc::PointerType *ty)
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

llvm::ArrayType *ObjBuilder::getArrayType(splc::ArrayType *ty)
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

llvm::Type *ObjBuilder::getCvtType(splc::Type *ty)
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

//===----------------------------------------------------------------------===//
//                               Code Generation
//===----------------------------------------------------------------------===//

//===----------------------------------------------------------------------===//
// Helper Functions

llvm::Function *ObjBuilder::getFunction(std::string_view name)
{
    if (auto *f = theModule->getFunction(name)) {
        return f;
    }

    // auto fi = functionProtos.find(name);
    // if (fi != functionProtos.end())
    //     return CGFuncProto(fi->second.second);

    splc_error() << "function should be present upon searching. Since "
                    "ObjBuilder registers all function prototypes in advance, "
                    "this indicates that the given AST is not "
                    "semantically correct: the name of the missing function is "
                 << name;

    return nullptr;
}

void ObjBuilder::registerGlobalCtxMutableVar(std::string_view name,
                                             const SymbolEntry &ent)
{
    splc_ilog_error(&ent.location, false)
        << "using global variables is currently unsupported";
    return;

    llvm::Type *ty = getCvtType(ent.type);
    llvm::Function *theFunction = builder->GetInsertBlock()->getParent();

    llvm::AllocaInst *alloca =
        createEntryBlockAlloc(theFunction, ty, nullptr, name);
    insertNamedValue(name, ty, alloca);
}

void ObjBuilder::registerCtxMutableVar(std::string_view name,
                                       const SymbolEntry &ent)
{
    llvm::Type *ty = getCvtType(ent.type);
    llvm::Function *theFunction = builder->GetInsertBlock()->getParent();

    llvm::AllocaInst *alloca =
        createEntryBlockAlloc(theFunction, ty, nullptr, name);
    insertNamedValue(name, ty, alloca);
}

// void ObjBuilder::registerCtxFuncParam(std::string_view name,
//                                       const SymbolEntry &ent)
// {
//     llvm::Type *ty = getCvtType(ent.type);
//     llvm::Function *theFunction = builder->GetInsertBlock()->getParent();

//     llvm::AllocaInst *alloca =
//         createEntryBlockAlloc(theFunction, ty, nullptr, name);
//     insertNamedValue(name, ty, alloca);
// }

void ObjBuilder::registerCtxFuncProto(std::string_view name,
                                      const SymbolEntry &ent)
{
    llvm::FunctionType *FT =
        getFunctionType(static_cast<FunctionType *>(ent.type));
    llvm::Function *theFunction = llvm::Function::Create(
        FT, llvm::Function::ExternalLinkage, name, theModule.get());

    std::vector<std::string_view> argNames;

    if (ent.body != nullptr) {
        auto ctx = ent.body->getASTContext();

        if (ctx != nullptr) {
            for (const auto &symListEnt : ctx->getSymbolList()) {
                const auto &symName = symListEnt.first;
                const auto &sym = symListEnt.second;
                if (sym.symEntTy == SymEntryType::Paramater) {
                    argNames.push_back(symName);
                }
            }

            unsigned idx = 0;
            for (auto &arg : theFunction->args()) {
                arg.setName(argNames[idx++]);
            }
        }
    }

    registerFuncProto(name, FT, ent.body);
}

void ObjBuilder::registerCtxFuncDef(std::string_view name,
                                    const SymbolEntry &ent)
{
    llvm::FunctionType *FT =
        getFunctionType(static_cast<FunctionType *>(ent.type));

    llvm::Function *theFunction = llvm::Function::Create(
        FT, llvm::Function::ExternalLinkage, name, theModule.get());

    std::vector<std::string_view> argNames;

    auto &protoNode = ent.body->getChildren()[0];
    auto ctx = protoNode->getASTContext();
    splc_dbgassert(ctx != nullptr);

    for (const auto &symListEnt : ctx->getSymbolList()) {
        const auto &symName = symListEnt.first;
        const auto &sym = symListEnt.second;
        if (sym.symEntTy == SymEntryType::Paramater) {
            argNames.push_back(symName);
        }
    }

    splc_dbgassert(argNames.size() == theFunction->arg_size());
    unsigned idx = 0;
    for (auto &arg : theFunction->args()) {
        arg.setName(argNames[idx++]);
    }

    registerFuncProto(name, FT, ent.body);
}

void ObjBuilder::registerCtx(Ptr<ASTContext> ctx)
{
    for (const auto &symListEnt : ctx->getSymbolList()) {
        const auto &symName = symListEnt.first;
        const auto &sym = symListEnt.second;

        switch (sym.symEntTy) {
        case SymEntryType::Unspecified:
        case SymEntryType::All:
        case SymEntryType::Empty: {
            break;
        }
        case SymEntryType::Function: {
            if (sym.defined) {
                registerCtxFuncDef(symName, sym);
            }
            else {
                registerCtxFuncProto(symName, sym);
            }
            break;
        }
        case SymEntryType::StructDecl: {
            // TODO(future)
            break;
        }
        case SymEntryType::UnionDecl: {
            // TODO(future)
            break;
        }
        case SymEntryType::EnumDecl: {
            // TODO(future)
            break;
        }
        case SymEntryType::Typedef: {
            // TODO(future)
            break;
        }
        case SymEntryType::Variable: {
            if (isVarCtxGlobalScope())
                registerGlobalCtxMutableVar(symName, sym);
            else
                registerCtxMutableVar(symName, sym);
            break;
        }
        case SymEntryType::Paramater:
            // SKIP
            break;
        }
    }
}

//===----------------------------------------------------------------------===//
// Expressions

llvm::Value *ObjBuilder::CGConstant(Ptr<AST> constantRoot)
{
    splc_dbgassert(constantRoot->isConstant());

    auto &child = constantRoot->getChildren()[0];

    switch (child->getSymType()) {
    case ASTSymType::UIntLiteral:
        // TODO(near_future): allow unsigned integer literal
        // TODO(near_future): allow int64
        return llvm::ConstantInt::get(
            llvm::Type::getInt32Ty(getLLVMCtx()),
            llvm::APInt(
                32, static_cast<ASTSIntType>(child->getConstVal<ASTUIntType>()),
                true));

    case ASTSymType::SIntLiteral:
        return llvm::ConstantInt::get(
            llvm::Type::getInt32Ty(getLLVMCtx()),
            llvm::APInt(32, child->getConstVal<ASTSIntType>(), true));

    case ASTSymType::FloatLiteral:
        return llvm::ConstantFP::get(
            getLLVMCtx(), llvm::APFloat(child->getConstVal<ASTFloatType>()));

    case ASTSymType::CharLiteral:
        return llvm::ConstantInt::get(
            getLLVMCtx(),
            llvm::APInt(ASTCharTypeNumBits, child->getConstVal<ASTCharType>()));

    default: {
        splc_ilog_error(&child->getLocation(), false)
            << "unsupported constant type: " << child->getSymType();
        return nullptr;
    }
    }
}

// llvm::Value *ObjBuilder::CGConstExpr(Ptr<AST> constExprRoot)
// {
//     // TODO!
// }

llvm::Value *ObjBuilder::CGPrimaryExpr(Ptr<AST> primaryExprRoot)
{
    splc_dbgassert(primaryExprRoot->isGeneralExpr());
    auto &child = primaryExprRoot->getChildren()[0];

    switch (child->getSymType()) {

    case ASTSymType::Constant:
        return CGConstant(child);

    case ASTSymType::Expr:
        return CGGeneralExprDispatch(child);

    case ASTSymType::ID:
        return CGExprID(child);

    case ASTSymType::StringLiteral: {
        // TODO(future)
        splc_ilog_error(&child->getLocation(), false)
            << "using string literals are not supported in ObjBuilder";
        return nullptr;
    }

    default: {
        splc_ilog_error(&primaryExprRoot->getLocation(), true)
            << "unsupported type for dispatch: "
            << primaryExprRoot->getSymType();
        return nullptr;
    }
    }
}

llvm::Value *ObjBuilder::CGPostfixExpr(Ptr<AST> postfixExprRoot)
{
    // splc_ilog_error(&postfixExprRoot->getLocation(), false)
    //     << "not supported in ObjBuilder";
    // return nullptr;
    // TODO(near_future) check for offset, allowing other postfix expressions

    splc_dbgassert(postfixExprRoot->getChildrenNum() == 2);
    auto &children = postfixExprRoot->getChildren();
    auto &opExpr = children[0];
    auto &op = children[1];
    llvm::Value *retVal = CGGeneralExprDispatch(opExpr);

    auto &IDNode = opExpr->getChildren()[0];
    splc_dbgassert(IDNode->isID());
    auto [ty, memloc] = findNamedValue(IDNode->getRootID());
    splc_dbgassert(memloc != nullptr);
    llvm::Value *newVal = nullptr;

    switch (op->getSymType()) {
    case ASTSymType::OpDPlus: {
        if (retVal->getType()->isIntegerTy()) {
            newVal = builder->CreateAdd(
                retVal,
                llvm::ConstantInt::get(
                    getLLVMCtx(),
                    llvm::APInt(retVal->getType()->getPrimitiveSizeInBits(),
                                1)));
        }
        else if (retVal->getType()->isFloatingPointTy()) {
            newVal = builder->CreateFAdd(
                retVal,
                llvm::ConstantFP::get(getLLVMCtx(), llvm::APFloat(1.0)));
        }
        break;
    }

    case ASTSymType::OpDMinus: {
        if (retVal->getType()->isIntegerTy()) {
            newVal = builder->CreateSub(
                retVal,
                llvm::ConstantInt::get(
                    getLLVMCtx(),
                    llvm::APInt(retVal->getType()->getPrimitiveSizeInBits(),
                                1)));
        }
        else if (retVal->getType()->isFloatingPointTy()) {
            newVal = builder->CreateFSub(
                retVal,
                llvm::ConstantFP::get(getLLVMCtx(), llvm::APFloat(1.0)));
        }
        break;
    }

    default: {
        splc_ilog_error(&op->getLocation(), false)
            << "unsupported op type: " << op->getSymType();
        return nullptr;
    }

    } // switch (op->getSymType())

    splc_dbgassert(newVal != nullptr);
    builder->CreateStore(newVal, memloc);
    return retVal;
}

llvm::Value *ObjBuilder::CGUnaryExpr(Ptr<AST> unaryExprRoot)
{
    splc_dbgassert(unaryExprRoot->getChildrenNum() == 2);
    auto &children = unaryExprRoot->getChildren();
    auto &op = children[0];
    auto &opExpr = children[1];
    llvm::Value *exprRes = CGGeneralExprDispatch(opExpr);

    // TODO(future): support sizeof
    splc_dbgassert(opExpr->isGeneralExpr());

    switch (op->getSymType()) {

    case ASTSymType::OpDPlus: {
        if (exprRes->getType()->isIntegerTy()) {
            return builder->CreateAdd(
                exprRes, llvm::ConstantInt::get(
                             getLLVMCtx(), llvm::APInt(ASTSIntTypeNumBits, 1)));
        }
        else if (exprRes->getType()->isFloatingPointTy()) {
            return builder->CreateFAdd(
                exprRes,
                llvm::ConstantFP::get(getLLVMCtx(), llvm::APFloat(1.0)));
        }
        break;
    }

    case ASTSymType::OpDMinus: {
        if (exprRes->getType()->isIntegerTy()) {
            return builder->CreateSub(
                exprRes, llvm::ConstantInt::get(
                             getLLVMCtx(), llvm::APInt(ASTSIntTypeNumBits, 1)));
        }
        else if (exprRes->getType()->isFloatingPointTy()) {
            return builder->CreateFSub(
                exprRes,
                llvm::ConstantFP::get(getLLVMCtx(), llvm::APFloat(1.0)));
        }
        break;
    }

    case ASTSymType::OpPlus: {
        return exprRes;
    }

    case ASTSymType::OpMinus: {
        if (exprRes->getType()->isIntegerTy()) {
            return builder->CreateNeg(exprRes);
        }
        else if (exprRes->getType()->isFloatingPointTy()) {
            return builder->CreateFNeg(exprRes);
        }
        break;
    }

    case ASTSymType::OpBNot: {
        if (exprRes->getType()->isIntegerTy()) {
            return builder->CreateNot(exprRes);
        }
        else {
            splc_ilog_error(&unaryExprRoot->getLocation(), false)
                << "using unknown type with bitwise not";
            return nullptr;
        }
    }

    case ASTSymType::OpNot: {
        if (exprRes->getType()->isIntegerTy()) {
            auto size_in_bits = exprRes->getType()->getPrimitiveSizeInBits();
            return builder->CreateICmpEQ(
                exprRes,
                llvm::ConstantInt::get(
                    builder->getIntNTy((unsigned)size_in_bits), 0, false));
        }
        else {
            splc_ilog_error(&unaryExprRoot->getLocation(), false)
                << "using unknown type with logical not";
            return nullptr;
        }
    }

    default:
        break;

    } // switch (op->getSymType())

    splc_ilog_error(&op->getLocation(), false)
        << "unsupported op type: " << op->getSymType();
    return nullptr;
}

llvm::Value *ObjBuilder::CGSubscriptExpr(Ptr<AST> subscriptExprRoot)
{
    // TODO(future): support
    splc_ilog_error(&subscriptExprRoot->getLocation(), false)
        << "subscription is not supported yet";
    return nullptr;
}

llvm::Value *ObjBuilder::CGDerefExpr(Ptr<AST> derefExprRoot)
{
    // TODO(future): support
    splc_ilog_error(&derefExprRoot->getLocation(), false)
        << "deref is not supported";
    return nullptr;
}

llvm::Value *ObjBuilder::CGAddrOfExpr(Ptr<AST> addrOfExprRoot)
{
    // TODO(future): support
    splc_ilog_error(&addrOfExprRoot->getLocation(), false)
        << "addr-of is not supported";
    return nullptr;
}

llvm::Value *ObjBuilder::CGAccessExpr(Ptr<AST> accessExprRoot)
{
    // TODO(near_future): support
    splc_ilog_error(&accessExprRoot->getLocation(), false)
        << "member access is not supported yet";
    return nullptr;
}

llvm::Value *ObjBuilder::CGSizeOfExpr(Ptr<AST> sizeOfExprRoot)
{
    // TODO(future): support
    splc_ilog_error(&sizeOfExprRoot->getLocation(), false)
        << "size-of is not supported";
    return nullptr;
}

llvm::Value *ObjBuilder::CGCallExpr(Ptr<AST> callExprRoot)
{
    splc_dbgassert(callExprRoot->isCallExpr());

    // Each argument is an expression
    // TODO(future): support arbitrary expression as function call callee

    auto &children = callExprRoot->getChildren();
    splc_dbgassert(children[0]->getChildrenNum() ==
                   1); // assert callee ID exists

    auto &calleeNode = children[0]->getChildren()[0];
    auto &argListNode = children[1];
    auto &argExprNodes = argListNode->getChildren();

    splc_dbgassert(calleeNode->isID());

    // Look up the name in the global table
    std::string calleeFName = calleeNode->getConstVal<ASTIDType>();
    llvm::Function *calleeF = getFunction(calleeFName);
    if (!calleeF) {
        splc_ilog_error(&callExprRoot->getLocation(), true)
            << "callee does not exist " << calleeFName;
        return nullptr;
    }

    if (calleeF->arg_size() != argListNode->getChildrenNum()) {
        splc_ilog_error(&callExprRoot->getLocation(), true)
            << "argument number does not match. Expected "
            << calleeF->arg_size() << ", got " << argListNode->getChildrenNum();
        return nullptr;
    }

    std::vector<llvm::Value *> argsV;
    argsV.reserve(argExprNodes.size());
    std::transform(argExprNodes.begin(), argExprNodes.end(),
                   std::back_inserter(argsV),
                   [this](const auto &p) { return CGGeneralExprDispatch(p); });

    // If the function has "void" as return type, then return nothing and do not
    // generate name for it.
    llvm::Type *ft = calleeF->getReturnType();
    if (ft->isVoidTy()) {
        builder->CreateCall(calleeF, argsV);
        return nullptr;
    }
    else {
        return builder->CreateCall(calleeF, argsV, "calltmp");
    }
}

llvm::Value *ObjBuilder::CGAssignExpr(Ptr<AST> assignExprRoot)
{
    splc_dbgassert(assignExprRoot->getChildrenNum() == 3);

    auto &children = assignExprRoot->getChildren();
    auto &lhsNode = children[0];
    auto &rhsNode = children[2];

    llvm::Value *lhsVal = CGGeneralExprDispatch(lhsNode);
    llvm::Value *rhsVal = CGGeneralExprDispatch(rhsNode);
    llvm::Value *val2BeAssigned = rhsVal;

    switch (children[1]->getSymType()) {

    case ASTSymType::OpAssign:
        break;

    case ASTSymType::OpMulAssign: {
        if (lhsVal->getType()->isIntegerTy()) {
            val2BeAssigned = builder->CreateMul(lhsVal, rhsVal);
        }
        else if (lhsVal->getType()->isFloatingPointTy()) {
            val2BeAssigned = builder->CreateFMul(lhsVal, rhsVal);
        }
        else {
            splc_ilog_error(nullptr, false) << "type mismatch";
        }
        break;
    }

    // TODO(near_future): relax the assumption all are signed integers
    case ASTSymType::OpDivAssign: {
        if (lhsVal->getType()->isIntegerTy()) {
            val2BeAssigned = builder->CreateSDiv(lhsVal, rhsVal);
        }
        else if (lhsVal->getType()->isFloatingPointTy()) {
            val2BeAssigned = builder->CreateFDiv(lhsVal, rhsVal);
        }
        else {
            splc_ilog_error(nullptr, false) << "type mismatch";
        }
        break;
    }

    case ASTSymType::OpModAssign: {
        if (lhsVal->getType()->isIntegerTy()) {
            auto tmp1 = builder->CreateSDiv(lhsVal, rhsVal);
            auto tmp2 = builder->CreateMul(rhsVal, tmp1);
            val2BeAssigned = builder->CreateSub(lhsVal, tmp2);
        }
        else {
            splc_ilog_error(nullptr, false) << "type mismatch or unsupported";
        }
        break;
    }

    case ASTSymType::OpPlusAssign: {
        if (lhsVal->getType()->isIntegerTy()) {
            val2BeAssigned = builder->CreateAdd(lhsVal, rhsVal);
        }
        else if (lhsVal->getType()->isFloatingPointTy()) {
            val2BeAssigned = builder->CreateFAdd(lhsVal, rhsVal);
        }
        else {
            splc_ilog_error(nullptr, false) << "type mismatch";
        }
        break;
    }

    case ASTSymType::OpMinusAssign: {
        if (lhsVal->getType()->isIntegerTy()) {
            val2BeAssigned = builder->CreateSub(lhsVal, rhsVal);
        }
        else if (lhsVal->getType()->isFloatingPointTy()) {
            val2BeAssigned = builder->CreateFSub(lhsVal, rhsVal);
        }
        else {
            splc_ilog_error(nullptr, false) << "type mismatch";
        }
        break;
    }

    case ASTSymType::OpLShiftAssign: {
        if (lhsVal->getType()->isIntegerTy()) {
            val2BeAssigned = builder->CreateShl(lhsVal, rhsVal);
        }
        else {
            splc_ilog_error(nullptr, false) << "type mismatch or unsupported";
        }
        break;
    }

    case ASTSymType::OpRShiftAssign: {
        if (lhsVal->getType()->isIntegerTy()) {
            val2BeAssigned = builder->CreateLShr(lhsVal, rhsVal);
        }
        else {
            splc_ilog_error(nullptr, false) << "type mismatch or unsupported";
        }
        break;
    }

    case ASTSymType::OpBAndAssign: {
        if (lhsVal->getType()->isIntegerTy()) {
            val2BeAssigned = builder->CreateAnd(lhsVal, rhsVal);
        }
        else {
            splc_ilog_error(nullptr, false) << "type mismatch or unsupported";
        }
        break;
    }

    case ASTSymType::OpBXorAssign: {
        if (lhsVal->getType()->isIntegerTy()) {
            val2BeAssigned = builder->CreateXor(lhsVal, rhsVal);
        }
        else {
            splc_ilog_error(nullptr, false) << "type mismatch or unsupported";
        }
        break;
    }

    case ASTSymType::OpBOrAssign: {
        if (lhsVal->getType()->isIntegerTy()) {
            val2BeAssigned = builder->CreateOr(lhsVal, rhsVal);
        }
        else {
            splc_ilog_error(nullptr, false) << "type mismatch or unsupported";
        }
        break;
    }

    default: {
        splc_ilog_error(&children[1]->getLocation(), false)
            << "unrecognized op type: " << children[1]->getSymType();
        break;
    }
    }

    auto &IDNode = lhsNode->getChildren()[0];
    splc_dbgassert(IDNode->isID());
    auto [ty, memloc] = findNamedValue(IDNode->getRootID());
    // llvm::AllocaInst *memloc = llvm::findAllocaForValue(lhsVal);
    // TODO(near_future) check for offset, allowing array specification
    splc_dbgassert(memloc != nullptr);
    builder->CreateStore(val2BeAssigned, memloc);

    return builder->CreateLoad(ty, memloc);
}

llvm::Value *ObjBuilder::CGBinaryCondExpr(Ptr<AST> binCondExprRoot)
{
    splc_dbgassert(binCondExprRoot->getChildrenNum() == 3);
    auto &children = binCondExprRoot->getChildren();
    auto opType = children[1]->getSymType();

    llvm::Value *lhsVal = CGGeneralExprDispatch(children[0]);
    llvm::Value *rhsVal = CGGeneralExprDispatch(children[2]);

    switch (opType) {
    case ASTSymType::OpAnd:
        return builder->CreateLogicalAnd(lhsVal, rhsVal);
    case ASTSymType::OpOr:
        return builder->CreateLogicalOr(lhsVal, rhsVal);
    case ASTSymType::OpLT: {
        if (lhsVal->getType()->isIntegerTy()) {
            // TODO(near_future): relax the assumption that integers are all
            // signed
            return builder->CreateICmpSLT(lhsVal, rhsVal);
        }
        else if (lhsVal->getType()->isFloatingPointTy()) {
            return builder->CreateFCmpOLT(lhsVal, rhsVal);
        }
        break;
    }
    case ASTSymType::OpLE: {
        if (lhsVal->getType()->isIntegerTy()) {
            // TODO(near_future): relax the assumption that integers are all
            // signed
            return builder->CreateICmpSLE(lhsVal, rhsVal);
        }
        else if (lhsVal->getType()->isFloatingPointTy()) {
            return builder->CreateFCmpOLE(lhsVal, rhsVal);
        }
        break;
    }
    case ASTSymType::OpGT: {
        if (lhsVal->getType()->isIntegerTy()) {
            // TODO(near_future): relax the assumption that integers are all
            // signed
            return builder->CreateICmpSGT(lhsVal, rhsVal);
        }
        else if (lhsVal->getType()->isFloatingPointTy()) {
            return builder->CreateFCmpOGT(lhsVal, rhsVal);
        }
        break;
    }
    case ASTSymType::OpGE: {
        if (lhsVal->getType()->isIntegerTy()) {
            // TODO(near_future): relax the assumption that integers are all
            // signed
            return builder->CreateICmpSGE(lhsVal, rhsVal);
        }
        else if (lhsVal->getType()->isFloatingPointTy()) {
            return builder->CreateFCmpOGE(lhsVal, rhsVal);
        }
        break;
    }
    case ASTSymType::OpNE: {
        if (lhsVal->getType()->isIntegerTy()) {
            // TODO(near_future): relax the assumption that integers are all
            // signed
            return builder->CreateICmpNE(lhsVal, rhsVal);
        }
        else if (lhsVal->getType()->isFloatingPointTy()) {
            return builder->CreateFCmpONE(lhsVal, rhsVal);
        }
        break;
    }
    case ASTSymType::OpEQ: {
        if (lhsVal->getType()->isIntegerTy()) {
            // TODO(near_future): relax the assumption that integers are all
            // signed
            return builder->CreateICmpEQ(lhsVal, rhsVal);
        }
        else if (lhsVal->getType()->isFloatingPointTy()) {
            return builder->CreateFCmpOEQ(lhsVal, rhsVal);
        }
        break;
    }

    default:
        break;
    }
    splc_ilog_error(&binCondExprRoot->getLocation(), false)
        << "unexpected operand type for operands: " << opType;
    return nullptr;
}

llvm::Value *ObjBuilder::CGBinaryArithExpr(Ptr<AST> binaryExprRoot)
{
    splc_dbgassert(binaryExprRoot->getChildrenNum() == 3);
    auto &children = binaryExprRoot->getChildren();
    auto opType = children[1]->getSymType();

    llvm::Value *lhsVal = CGGeneralExprDispatch(children[0]);
    llvm::Value *rhsVal = CGGeneralExprDispatch(children[2]);

    switch (opType) {
    case ASTSymType::OpLShift: {
        if (lhsVal->getType()->isIntegerTy()) {
            // TODO(near_future): relax the assumption that integers are all
            // signed
            return builder->CreateShl(lhsVal, rhsVal);
        }
        break;
    }

    case ASTSymType::OpRShift: {
        if (lhsVal->getType()->isIntegerTy()) {
            // TODO(near_future): relax the assumption that integers are all
            // signed
            return builder->CreateLShr(lhsVal, rhsVal);
        }
        break;
    }

    case ASTSymType::OpBAnd: {
        if (lhsVal->getType()->isIntegerTy()) {
            // TODO(near_future): relax the assumption that integers are all
            // signed
            return builder->CreateAnd(lhsVal, rhsVal);
        }
        break;
    }

    case ASTSymType::OpBOr: {
        if (lhsVal->getType()->isIntegerTy()) {
            // TODO(near_future): relax the assumption that integers are all
            // signed
            return builder->CreateOr(lhsVal, rhsVal);
        }
        break;
    }

    case ASTSymType::OpBXor: {
        if (lhsVal->getType()->isIntegerTy()) {
            // TODO(near_future): relax the assumption that integers are all
            // signed
            return builder->CreateXor(lhsVal, rhsVal);
        }
        break;
    }

    case ASTSymType::OpPlus: {
        if (lhsVal->getType()->isIntegerTy()) {
            // TODO(near_future): relax the assumption that integers are all
            // signed
            return builder->CreateAdd(lhsVal, rhsVal);
        }
        else if (lhsVal->getType()->isFloatingPointTy()) {
            return builder->CreateFAdd(lhsVal, rhsVal);
        }
        break;
    }

    case ASTSymType::OpMinus: {
        if (lhsVal->getType()->isIntegerTy()) {
            // TODO(near_future): relax the assumption that integers are all
            // signed
            return builder->CreateSub(lhsVal, rhsVal);
        }
        else if (lhsVal->getType()->isFloatingPointTy()) {
            return builder->CreateFSub(lhsVal, rhsVal);
        }
        break;
    }

    case ASTSymType::OpAstrk: {
        if (lhsVal->getType()->isIntegerTy()) {
            // TODO(near_future): relax the assumption that integers are all
            // signed
            return builder->CreateMul(lhsVal, rhsVal);
        }
        else if (lhsVal->getType()->isFloatingPointTy()) {
            return builder->CreateFMul(lhsVal, rhsVal);
        }
        break;
    }

    case ASTSymType::OpDiv: {
        if (lhsVal->getType()->isIntegerTy()) {
            // TODO(near_future): relax the assumption that integers are all
            // signed
            return builder->CreateSDiv(lhsVal, rhsVal);
        }
        else if (lhsVal->getType()->isFloatingPointTy()) {
            return builder->CreateFDiv(lhsVal, rhsVal);
        }
        break;
    }

    case ASTSymType::OpMod: {
        if (lhsVal->getType()->isIntegerTy()) {
            // TODO(near_future): relax the assumption that integers are all
            // signed
            auto tmp1 = builder->CreateSDiv(lhsVal, rhsVal);
            auto tmp2 = builder->CreateMul(rhsVal, tmp1);
            return builder->CreateSub(lhsVal, tmp2);
        }
        break;
    }

    default:
        break;
    }

    splc_ilog_error(&binaryExprRoot->getLocation(), false)
        << "unexpected operand type for operands: " << opType;
    return nullptr;
}

llvm::Value *ObjBuilder::CGTrinaryCondExpr(Ptr<AST> triCondExprRoot)
{
    // TODO(future)
    splc_ilog_error(&triCondExprRoot->getLocation(), false)
        << "trinary expression not supported yet";
    return nullptr;
}

llvm::Value *ObjBuilder::CGImplicitCastExpr(Ptr<AST> impCastExprRoot)
{
    // TODO(future)
    splc_ilog_error(&impCastExprRoot->getLocation(), false)
        << "implicit casts are not supported in ObjBuilder";
    return nullptr;
}

llvm::Value *ObjBuilder::CGExplicitCastExpr(Ptr<AST> expCastExprRoot)
{
    // TODO(future)
    splc_ilog_error(&expCastExprRoot->getLocation(), false)
        << "explicit casts are not supported in ObjBuilder";
    return nullptr;
}

llvm::Value *ObjBuilder::CGExprID(Ptr<AST> IDRoot)
{
    splc_dbgassert(IDRoot->isID());
    auto name = IDRoot->getConstVal<ASTIDType>();
    auto [ty, allocaInst] = findNamedValue(name);
    return builder->CreateLoad(ty, allocaInst, name);
}

llvm::Value *ObjBuilder::CGGeneralExprDispCN1(Ptr<AST> exprRoot)
{
    auto &child = exprRoot->getChildren()[0];

    switch (child->getSymType()) {

    case ASTSymType::Constant:
    case ASTSymType::Expr:
    case ASTSymType::ID:
    case ASTSymType::StringLiteral:
        return CGPrimaryExpr(exprRoot);

    default: {
        splc_ilog_error(&child->getLocation(), true)
            << "unsupported type for dispatch: " << child->getSymType();
        return nullptr;
    }
    }
}

llvm::Value *ObjBuilder::CGGeneralExprDispCN2(Ptr<AST> exprRoot)
{
    auto &children = exprRoot->getChildren();

    if (children[1]->isSymTypeOneOf(ASTSymType::OpDPlus, ASTSymType::OpDMinus))
        return CGPostfixExpr(exprRoot);

    if (children[0]->isSymTypeOneOf(ASTSymType::OpDPlus, ASTSymType::OpDMinus,
                                    ASTSymType::OpPlus, ASTSymType::OpMinus,
                                    ASTSymType::OpBNot, ASTSymType::OpNot))
        return CGUnaryExpr(exprRoot);

    splc_ilog_error(&exprRoot->getLocation(), false)
        << "unsupported type for ObjBuilder: " << exprRoot->getSymType();
    return nullptr;
}

llvm::Value *ObjBuilder::CGGeneralExprDispCN3(Ptr<AST> exprRoot)
{
    auto &children = exprRoot->getChildren();

    if (children[1]->isSymTypeOneOf(
            ASTSymType::OpAssign, ASTSymType::OpMulAssign,
            ASTSymType::OpDivAssign, ASTSymType::OpModAssign,
            ASTSymType::OpPlusAssign, ASTSymType::OpMinusAssign,
            ASTSymType::OpLShiftAssign, ASTSymType::OpRShiftAssign,
            ASTSymType::OpBAndAssign, ASTSymType::OpBXorAssign,
            ASTSymType::OpBOrAssign))
        return CGAssignExpr(exprRoot);

    if (children[1]->isSymTypeOneOf(
            ASTSymType::OpAnd, ASTSymType::OpOr, ASTSymType::OpNot,
            ASTSymType::OpLT, ASTSymType::OpLE, ASTSymType::OpGT,
            ASTSymType::OpGE, ASTSymType::OpNE, ASTSymType::OpEQ))
        return CGBinaryCondExpr(exprRoot);

    if (children[1]->isSymTypeOneOf(
            ASTSymType::OpLShift, ASTSymType::OpRShift, ASTSymType::OpBAnd,
            ASTSymType::OpBOr, ASTSymType::OpBNot, ASTSymType::OpBXor,
            ASTSymType::OpPlus, ASTSymType::OpMinus, ASTSymType::OpAstrk,
            ASTSymType::OpDiv, ASTSymType::OpMod))
        return CGBinaryArithExpr(exprRoot);

    if (children[1]->isOpComma()) {
        llvm::Value *ret = CGGeneralExprDispatch(children[0]);
        CGGeneralExprDispatch(children[2]);
        return ret;
    }

    splc_ilog_error(&exprRoot->getLocation(), false)
        << "unsupported type for ObjBuilder: " << exprRoot->getSymType();
    return nullptr;
}

llvm::Value *ObjBuilder::CGGeneralExprDispCNN(Ptr<AST> exprRoot)
{
    splc_dbgassert(exprRoot->getChildrenNum() > 0);
    auto &children = exprRoot->getChildren();

    if (children[1]->isOpQMark())
        return CGTrinaryCondExpr(exprRoot);

    splc_ilog_error(&exprRoot->getLocation(), false)
        << "unsupported type dispatch: " << exprRoot->getSymType();
    return nullptr;
}

llvm::Value *ObjBuilder::CGGeneralExprDispatch(Ptr<AST> exprRoot)
{
    splc_dbgassert(exprRoot->isGeneralExpr());

    switch (exprRoot->getSymType()) {

    case ASTSymType::ExplicitCastExpr:
        return CGExplicitCastExpr(exprRoot);

    case ASTSymType::ImplicitCastExpr:
        return CGImplicitCastExpr(exprRoot);

    case ASTSymType::AddrOfExpr:
        return CGAddrOfExpr(exprRoot);

    case ASTSymType::DerefExpr:
        return CGDerefExpr(exprRoot);

    case ASTSymType::SubscriptExpr:
        return CGSubscriptExpr(exprRoot);

    case ASTSymType::AccessExpr:
        return CGAccessExpr(exprRoot);

    case ASTSymType::CallExpr:
        return CGCallExpr(exprRoot);

    case ASTSymType::SizeOfExpr:
        return CGSizeOfExpr(exprRoot);

    default:
        break;
    }

    auto N = exprRoot->getChildrenNum();
    switch (N) {
    case 1:
        return CGGeneralExprDispCN1(exprRoot);
    case 2:
        return CGGeneralExprDispCN2(exprRoot);
    case 3:
        return CGGeneralExprDispCN3(exprRoot);
    default:
        return CGGeneralExprDispCNN(exprRoot);
    }
}

//===----------------------------------------------------------------------===//
// Statements

void ObjBuilder::CGCompStmt(Ptr<AST> compStmtRoot)
{
    splc_dbgassert(compStmtRoot->isCompStmt());

    auto &children = compStmtRoot->getChildren();
    if (children.size() == 0) {
        return;
    }

    auto &genStmtList = children[0];

    pushVarCtxStack();
    registerCtx(compStmtRoot->getASTContext());

    for (auto &genChild : genStmtList->getChildren()) {
        if (genChild->isStmt()) {
            CGStmt(genChild);
        }
        if (genChild->isDecl()) {
            CGDecl(genChild);
        }
    }

    popVarCtxStack();
}

void ObjBuilder::CGExprStmt(Ptr<AST> exprStmtRoot)
{
    splc_dbgassert(exprStmtRoot->isExprStmt());
    auto &expr = exprStmtRoot->getChildren()[0];
    CGGeneralExprDispatch(expr);
}

void ObjBuilder::CGIfStmt(Ptr<AST> ifStmtRoot)
{
    splc_dbgassert(ifStmtRoot->isSelStmt());

    auto &children = ifStmtRoot->getChildren();
    auto &cond = children[1];
    auto &thenStmt = children[2];

    llvm::Value *condV = CGGeneralExprDispatch(cond);

    condV = builder->CreateICmpNE(
        condV,
        llvm::ConstantInt::get(
            getLLVMCtx(),
            llvm::APInt(condV->getType()->getPrimitiveSizeInBits(), 0)),
        "ifcond");

    llvm::Function *theFunction = builder->GetInsertBlock()->getParent();

    llvm::BasicBlock *thenBB =
        llvm::BasicBlock::Create(getLLVMCtx(), "then", theFunction);
    llvm::BasicBlock *elseBB = llvm::BasicBlock::Create(getLLVMCtx(), "else");
    llvm::BasicBlock *mergeBB =
        llvm::BasicBlock::Create(getLLVMCtx(), "ifcont");

    // Emit starting conditional
    builder->CreateCondBr(condV, thenBB, elseBB);

    // Emit IfStmt
    builder->SetInsertPoint(thenBB);
    CGStmt(thenStmt);

    // Get the new thenBB in case it has changed
    thenBB = builder->GetInsertBlock();
    llvm::Instruction *thenTerm = thenBB->getTerminator();
    if (thenTerm == nullptr)
        builder->CreateBr(mergeBB); // LLVM IR requires each basic block to end
                                    // with jump/other instructions

    // Emit ElseStmt
    theFunction->insert(theFunction->end(), elseBB);
    builder->SetInsertPoint(elseBB);
    if (children.size() == 5) {
        // If with Else
        auto &elseStmt = children[4];
        CGStmt(elseStmt);
    }

    // Get the new elseBB in case it has changed
    elseBB = builder->GetInsertBlock();

    // If there is already a terminator, then do nothing
    llvm::Instruction *elseTerm = elseBB->getTerminator();
    if (elseTerm == nullptr)
        builder->CreateBr(mergeBB);

    theFunction->insert(theFunction->end(), mergeBB);
    builder->SetInsertPoint(mergeBB);

    // No PHI node because they don't return llvm::Value*
}

void ObjBuilder::CGSelStmt(Ptr<AST> selStmtRoot)
{
    splc_dbgassert(selStmtRoot->isSelStmt());

    auto &children = selStmtRoot->getChildren();

    if (children[0]->isKwdIf()) {
        CGIfStmt(selStmtRoot);
        return;
    }

    // TODO(near_future)
    splc_ilog_error(&selStmtRoot->getLocation(), false)
        << "not supported in ObjBuilder";
}

void ObjBuilder::CGDoWhileStmt(Ptr<AST> doWhileStmtRoot)
{
    splc_dbgassert(doWhileStmtRoot->isIterStmt());
    // TODO(near_future)
    splc_ilog_error(&doWhileStmtRoot->getLocation(), false)
        << "not supported in ObjBuilder";
}

void ObjBuilder::CGWhileStmt(Ptr<AST> whileStmtRoot)
{
    splc_dbgassert(whileStmtRoot->isIterStmt());

    auto &children = whileStmtRoot->getChildren();
    splc_dbgassert(children.size() == 3) << "should be KwdWhile Expr Stmt";

    auto &expr = children[1];
    auto &stmt = children[2];

    llvm::Function *theFunction = builder->GetInsertBlock()->getParent();

    llvm::BasicBlock *loopTestBB =
        llvm::BasicBlock::Create(getLLVMCtx(), "looptest", theFunction);
    llvm::BasicBlock *loopBodyBB =
        llvm::BasicBlock::Create(getLLVMCtx(), "loopbody");
    llvm::BasicBlock *afterBB =
        llvm::BasicBlock::Create(getLLVMCtx(), "afterloop");

    builder->CreateBr(loopTestBB);
    builder->SetInsertPoint(loopTestBB);

    llvm::Value *condV = CGGeneralExprDispatch(expr);
    condV = builder->CreateICmpNE(
        condV,
        llvm::ConstantInt::get(
            getLLVMCtx(),
            llvm::APInt(condV->getType()->getPrimitiveSizeInBits(), 0)),
        "ifcond");

    // Emit starting conditional
    builder->CreateCondBr(condV, loopBodyBB, afterBB);

    // Emit Body
    theFunction->insert(theFunction->end(), loopBodyBB);
    builder->SetInsertPoint(loopBodyBB);
    CGStmt(stmt);

    builder->CreateBr(loopTestBB);
    theFunction->insert(theFunction->end(), afterBB);
    builder->SetInsertPoint(afterBB);
}

void ObjBuilder::CGForStmt(Ptr<AST> forStmtRoot)
{
    splc_dbgassert(forStmtRoot->isIterStmt());
    // TODO(near_future)
    splc_ilog_error(&forStmtRoot->getLocation(), false)
        << "not supported in ObjBuilder";
}

void ObjBuilder::CGIterStmt(Ptr<AST> iterStmtRoot)
{
    splc_dbgassert(iterStmtRoot->isIterStmt());

    auto &children = iterStmtRoot->getChildren();

    if (children[0]->isKwdWhile()) {
        CGWhileStmt(iterStmtRoot);
        return;
    }

    // TODO(near_future)
    splc_ilog_error(&iterStmtRoot->getLocation(), false)
        << "not supported in ObjBuilder";
}

void ObjBuilder::CGLabeledStmt(Ptr<AST> labeledStmtRoot)
{
    splc_dbgassert(labeledStmtRoot->isLabeledStmt());
    // TODO(near_future)
    splc_ilog_error(&labeledStmtRoot->getLocation(), false)
        << "not supported in ObjBuilder";
}

void ObjBuilder::CGReturnStmt(Ptr<AST> returnStmtRoot)
{
    splc_dbgassert(returnStmtRoot->isJumpStmt());

    auto &children = returnStmtRoot->getChildren();

    if (children.size() == 1) {
        llvm::Type *retTy =
            builder->GetInsertBlock()->getParent()->getReturnType();
        if (retTy->isVoidTy())
            builder->CreateRetVoid();
        else
            builder->CreateRet(llvm::Constant::getNullValue(retTy));
        return;
    }

    llvm::Value *val = CGGeneralExprDispatch(children[1]);
    builder->CreateRet(val);
}

void ObjBuilder::CGJumpStmt(Ptr<AST> jumpStmtRoot)
{
    splc_dbgassert(jumpStmtRoot->isJumpStmt());

    auto &children = jumpStmtRoot->getChildren();

    if (children[0]->isKwdReturn()) {
        CGReturnStmt(jumpStmtRoot);
        return;
    }

    // TODO(near_future)
    splc_ilog_error(&jumpStmtRoot->getLocation(), false)
        << "not supported in ObjBuilder";
}

void ObjBuilder::CGStmt(Ptr<AST> stmtRoot)
{
    splc_dbgassert(stmtRoot->isStmt());

    if (stmtRoot->getChildrenNum() == 0)
        return;

    auto &child = stmtRoot->getChildren()[0];

    switch (child->getSymType()) {
    case ASTSymType::CompStmt: {
        CGCompStmt(child);
        break;
    }

    case ASTSymType::ExprStmt: {
        CGExprStmt(child);
        break;
    }

    case ASTSymType::SelStmt: {
        CGSelStmt(child);
        break;
    }

    case ASTSymType::IterStmt: {
        CGIterStmt(child);
        break;
    }

    case ASTSymType::LabeledStmt: {
        CGLabeledStmt(child);
        break;
    }

    case ASTSymType::JumpStmt: {
        CGJumpStmt(child);
        break;
    }

    default:
        splc_ilog_error(&child->getLocation(), false)
            << "unrecognized statement type: " << child->getSymType();
        break;
    }
}

//===----------------------------------------------------------------------===//
// Declarations

llvm::Function *ObjBuilder::CGFuncProto(Ptr<AST> protoRoot)
{
    // TODO(revise): just do nothing
    return nullptr;
}

llvm::Function *ObjBuilder::CGFuncDef(Ptr<AST> funcRoot)
{
    // The given node should be of ASTSymType::FuncDef
    splc_dbgassert(funcRoot->isFuncDef());

    auto &protoNode = funcRoot->getChildren()[0];
    auto &compStmtNode = funcRoot->getChildren()[1];
    auto &funcDecltrNode = protoNode->getChildren()[1];

    // Get the type of this function
    Type *splcFuncTy = funcDecltrNode->getLangType();
    llvm::Type *funcTy = getCvtType(splcFuncTy); // transform into llvm type

    auto ID = funcDecltrNode->getRootID();
    llvm::Function *theFunction = getFunction(ID);
    if (!theFunction) {
        return nullptr;
    }

    llvm::BasicBlock *BB =
        llvm::BasicBlock::Create(getLLVMCtx(), ID, theFunction);
    builder->SetInsertPoint(BB);

    pushVarCtxStack();
    registerCtx(protoNode->getASTContext());

    for (auto &arg : theFunction->args()) {
        // Create an alloca for this variable
        llvm::AllocaInst *alloca =
            createEntryBlockAlloc(theFunction, funcTy, nullptr, arg.getName());

        builder->CreateStore(&arg, alloca);

        insertNamedValue(arg.getName(), arg.getType(), alloca);
    }

    CGCompStmt(compStmtNode);

    popVarCtxStack();

    llvm::Instruction *funcEndInst = builder->GetInsertBlock()->getTerminator();
    if (funcEndInst == nullptr) {
        llvm::Type *retTy = theFunction->getReturnType();
        if (retTy->isVoidTy())
            builder->CreateRetVoid();
        else
            builder->CreateRet(llvm::Constant::getNullValue(retTy));
    }

    llvm::raw_os_ostream errorOut{std::cerr};
    if (llvm::verifyFunction(*theFunction, &errorOut)) {
        errorOut.flush();
        splc_ilog_error(&funcRoot->getLocation(), true)
            << "this function is not well-formed according to the LLVM "
               "backend. This may be caused by "
               "unreachable code inside the provided source code. Try remove "
               "them and compile again. This function has been ignored.";
        SPLC_LOG_NOTE(&funcRoot->getLocation(), false) << "first defined here";
        theFunction->eraseFromParent();
        setGenerationStatus(false);
        return nullptr;
    }

    return theFunction;
}

llvm::Value *ObjBuilder::CGInitializer(Ptr<AST> initRoot)
{
    splc_dbgassert(initRoot->isInitializer());
    // TODO(future): support all kinds of initializers

    auto &child = initRoot->getChildren()[0];

    if (!child->isGeneralExpr()) {
        splc_ilog_error(&initRoot->getLocation(), true)
            << "non (Assign)Expr node as initializer is not supported";
        return nullptr;
    }

    return CGGeneralExprDispatch(child);
}

void ObjBuilder::CGInitDecltr(Ptr<AST> initDecltrRoot)
{
    splc_dbgassert(initDecltrRoot->isInitDecltr());

    // Procedure:
    // 1. Check if it is declarator with initializer.
    // 2a. If not, return.
    // 2b. If indeed, first get the name of the declarator, then
    //     process the initializer
    if (initDecltrRoot->getChildrenNum() != 3)
        return;

    auto &decltrNode = initDecltrRoot->getChildren()[0];
    auto &initNode = initDecltrRoot->getChildren()[2];

    llvm::Value *initVal = CGInitializer(initNode);
    auto ID = decltrNode->getRootID();
    auto [ty, allocaInst] = findNamedValue(ID);
    splc_dbgassert(allocaInst != nullptr)
        << "cannot bind allocation instance to ID " << ID;
    builder->CreateStore(initVal, allocaInst);
}

void ObjBuilder::CGInitDecltrList(Ptr<AST> initDecltrListRoot)
{
    splc_dbgassert(initDecltrListRoot->isInitDecltrList());
    for (auto &child : initDecltrListRoot->getChildren()) {
        CGInitDecltr(child);
    }
}

void ObjBuilder::CGDirDecl(Ptr<AST> dirDeclRoot)
{
    splc_dbgassert(dirDeclRoot->isDirDecl());
    // Two possibilities:
    // - DeclSpecWrapper
    // - DeclSpecWrapper InitDecltrList
    if (auto child = dirDeclRoot->findFirstChild(ASTSymType::InitDecltrList);
        child != nullptr) {
        CGInitDecltrList(child);
    }
    else {
        // SKIP; Simple Declaration
    }
}

void ObjBuilder::CGDecl(Ptr<AST> declRoot)
{
    splc_dbgassert(declRoot->isDecl());
    auto &child0 = declRoot->getChildren()[0];
    CGDirDecl(child0);
}

void ObjBuilder::CGExternDecl(Ptr<AST> externDeclRoot)
{
    splc_dbgassert(externDeclRoot->isExternDecl());
    auto &child0 = externDeclRoot->getChildren()[0];
    switch (child0->getSymType()) {
    case ASTSymType::Decl: {
        CGDecl(child0);
        break;
    }
    case ASTSymType::FuncDef: {
        CGFuncDef(child0);
        break;
    }
    default:
        splc_ilog_error(&child0->getLocation(), false)
            << "unexpected extern declaration";
    }
}

void ObjBuilder::CGExternDeclList(Ptr<AST> externDeclListRoot)
{
    splc_dbgassert(externDeclListRoot->isExternDeclList());
    for (auto &child : externDeclListRoot->getChildren()) {
        CGExternDecl(child);
    }
}

void ObjBuilder::CGTransUnit(Ptr<AST> transUnitRoot)
{
    splc_dbgassert(transUnitRoot->isTransUnit());
    if (transUnitRoot->getChildrenNum() == 0)
        return;

    // Register the context of translation unit
    pushVarCtxStack();
    registerCtx(transUnitRoot->getASTContext());

    auto &child = transUnitRoot->getChildren()[0];
    CGExternDeclList(child);

    // Clean-up
    popVarCtxStack();
}

void ObjBuilder::generateModule(TranslationUnit &tunit)
{
    generateModuleImpl(tunit);
}

void ObjBuilder::optimizeModule() { optimizeModuleImpl(); }

void ObjBuilder::writeModuleAsLLVMIR(std::ostream &os)
{
    writeModuleLLVMIRImpl(os);
}

void ObjBuilder::writeModuleAsMIPSObj(std::string_view path)
{
    auto targetTriple = llvm::sys::getDefaultTargetTriple();
    writeModuleObjImpl("mips", path);
}

void ObjBuilder::writeModuleAsDefaultObj(std::string_view path)
{
    auto targetTriple = llvm::sys::getDefaultTargetTriple();
    writeModuleObjImpl(targetTriple, path);
}

//===----------------------------------------------------------------------===//
//                          IR/Obj Generation Impl
//===----------------------------------------------------------------------===//

void ObjBuilder::generateModuleImpl(TranslationUnit &tunit)
{
    initializeInternalStates();
    CGTransUnit(tunit.getRootNode());
    llvmModuleGenerated = true;
}

void ObjBuilder::optimizeModuleImpl()
{
    if (!llvmModuleGenerated) {
        splc_ilog_error(nullptr, false)
            << "contained module has not been generated";
        return;
    }

    // Create new pass and analysis managers.
    theFPM = makeUniquePtr<llvm::FunctionPassManager>();
    theLAM = makeUniquePtr<llvm::LoopAnalysisManager>();
    theFAM = makeUniquePtr<llvm::FunctionAnalysisManager>();
    theCGAM = makeUniquePtr<llvm::CGSCCAnalysisManager>();
    theMAM = makeUniquePtr<llvm::ModuleAnalysisManager>();
    thePIC = makeUniquePtr<llvm::PassInstrumentationCallbacks>();
    theSI =
        makeUniquePtr<llvm::StandardInstrumentations>(getLLVMCtx(),
                                                      /*DebugLogging*/ true);
    theSI->registerCallbacks(*thePIC, theMAM.get());

    // Add transform passes.
    // Do simple "peephole" optimizations and bit-twiddling optzns.
    theFPM->addPass(llvm::InstCombinePass());
    // Reassociate expressions.
    theFPM->addPass(llvm::ReassociatePass());
    // Eliminate Common SubExpressions.
    theFPM->addPass(llvm::GVNPass());
    // Simplify the control flow graph (deleting unreachable blocks, etc).
    theFPM->addPass(llvm::SimplifyCFGPass());

    // Register analysis passes used in these transform passes.
    llvm::PassBuilder PB;
    PB.registerModuleAnalyses(*theMAM);
    PB.registerFunctionAnalyses(*theFAM);
    PB.crossRegisterProxies(*theLAM, *theFAM, *theCGAM, *theMAM);

    // Optimize each function (if not declaration only)
    for (auto &func : theModule->getFunctionList()) {
        if (!func.isDeclaration())
            theFPM->run(func, *theFAM);
    }

    // TODO(near_future): module-level optimization
}

void ObjBuilder::writeModuleLLVMIRImpl(std::ostream &os)
{
    llvm::raw_os_ostream trueOs{os};
    theModule->print(trueOs, nullptr);
}

void ObjBuilder::writeModuleObjImpl(std::string_view targetTriple,
                                    std::string_view path)
{
    if (!llvmModuleGenerated && !isGenerationSuccess()) {
        splc_ilog_fatal_error(nullptr, false)
            << "no module generated/generation has failed. Skipping writing "
               "object file.";
        return;
    }

    initializeTargetRegistry();
    theModule->setTargetTriple(targetTriple);

    std::string errorMsg;
    auto target = llvm::TargetRegistry::lookupTarget(targetTriple, errorMsg);

    if (!target) {
        splc_ilog_fatal_error(nullptr, false)
            << "failed to lookup target: " << targetTriple;
        splc_ilog_fatal_error(nullptr, false) << errorMsg;
        return;
    }

    auto CPU = "generic";
    auto Features = "";

    llvm::TargetOptions opt;
    auto targetMachine = target->createTargetMachine(
        targetTriple, CPU, Features, opt, llvm::Reloc::PIC_);

    theModule->setDataLayout(targetMachine->createDataLayout());

    std::error_code errorCode;
    llvm::raw_fd_ostream dest(path, errorCode, llvm::sys::fs::OF_None);

    if (errorCode) {
        splc_ilog_fatal_error(nullptr, false)
            << "Could not open file: " << errorCode.message();
        delete targetMachine;
        return;
    }

    llvm::legacy::PassManager pass;
    auto fileType = llvm::CodeGenFileType::ObjectFile;

    if (targetMachine->addPassesToEmitFile(pass, dest, nullptr, fileType)) {
        splc_ilog_fatal_error(nullptr, false)
            << "TheTargetMachine can't emit a file of this type";
        delete targetMachine;
        return;
    }

    pass.run(*theModule);

    // cleanup
    dest.flush();
    delete targetMachine;

    using CS = utils::logging::ControlSeq;
    SPLC_LOG_INFO(nullptr, false)
        << "writing object file for platform: " << CS::BrightCyan
        << targetTriple << CS::Reset;
    SPLC_LOG_INFO(nullptr, false) << "wrote " << path;
}

//===----------------------------------------------------------------------===//
//                          Internal State Management
//===----------------------------------------------------------------------===//

void ObjBuilder::initializeModuleAndManagers()
{
    // Open a new context and module.
    llvmCtx = makeUniquePtr<llvm::LLVMContext>();
    theModule = makeUniquePtr<llvm::Module>(
        "splc auto-gen module " + std::to_string(moduleCnt++), getLLVMCtx());

    // Create a new builder for the module.
    builder = makeUniquePtr<llvm::IRBuilder<>>(getLLVMCtx());
}

void ObjBuilder::initializeTargetRegistry()
{
    if (llvmTargetEnvInitialized)
        return;

    llvm::InitializeAllTargetInfos();
    llvm::InitializeAllTargets();
    llvm::InitializeAllTargetMCs();
    llvm::InitializeAllAsmParsers();
    llvm::InitializeAllAsmPrinters();
    llvmTargetEnvInitialized = true;
}

void ObjBuilder::initializeInternalStates()
{
    tyCache.clear();
    varCtxStack.clear();
    functionProtos.clear();
    setGenerationStatus(true);
    llvmModuleGenerated = false;

    initializeModuleAndManagers();
}

//===----------------------------------------------------------------------===//
// Variable Management

llvm::AllocaInst *ObjBuilder::createEntryBlockAlloc(llvm::Function *theFunction,
                                                    llvm::Type *type,
                                                    llvm::Value *arrSize,
                                                    std::string_view varName)
{
    llvm::IRBuilder<> tmpB(&theFunction->getEntryBlock(),
                           theFunction->getEntryBlock().begin());
    return tmpB.CreateAlloca(type, arrSize, varName);
}

void ObjBuilder::pushVarCtxStack() { varCtxStack.push_back({}); }

void ObjBuilder::popVarCtxStack() { varCtxStack.pop_back(); }

std::pair<llvm::Type *, llvm::AllocaInst *>
ObjBuilder::findNamedValue(std::string_view name) const
{
    for (auto &varCtx : std::views::reverse(varCtxStack)) {
        auto it = varCtx.namedValues.find(name);
        if (it != varCtx.namedValues.end())
            return it->second;
    }
    return {nullptr, nullptr};
}

void ObjBuilder::insertNamedValue(std::string_view name, llvm::Type *ty,
                                  llvm::AllocaInst *alloca)
{
    auto &varCtx = varCtxStack.back();
    varCtx.namedValues[std::string{name}] = {ty, alloca};
}

void ObjBuilder::registerFuncProto(std::string_view name, llvm::Type *ty,
                                   Ptr<AST> protoRoot)
{
    functionProtos[std::string{name}] = {ty, protoRoot};
}

std::pair<llvm::Type *, Ptr<AST>>
ObjBuilder::findFuncProto(std::string_view name)
{
    auto it = functionProtos.find(name);
    if (it == functionProtos.end())
        return {nullptr, nullptr};
    return it->second;
}

} // namespace splc