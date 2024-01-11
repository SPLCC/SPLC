#include "CodeGen/ObjBuilder.hh"
#include <ranges>

namespace splc {

//===----------------------------------------------------------------------===//
//                          LLVMIRBuilder Implementation
//===----------------------------------------------------------------------===//

void ObjBuilder::initializeModuleAndManagers()
{
    // Open a new context and module.
    theModule = std::make_unique<llvm::Module>(
        "module" + std::to_string(moduleCnt++), llvmCtx);

    // Create a new builder for the module.
    theBuilder = std::make_unique<llvm::IRBuilder<>>(llvmCtx);

    // Create new pass and analysis managers.
    theFPM = std::make_unique<llvm::FunctionPassManager>();
    theLAM = std::make_unique<llvm::LoopAnalysisManager>();
    theFAM = std::make_unique<llvm::FunctionAnalysisManager>();
    theCGAM = std::make_unique<llvm::CGSCCAnalysisManager>();
    theMAM = std::make_unique<llvm::ModuleAnalysisManager>();
    thePIC = std::make_unique<llvm::PassInstrumentationCallbacks>();
    theSI =
        std::make_unique<llvm::StandardInstrumentations>(llvmCtx,
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
}

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
    for (auto i = 1; i < splcSubTys.size(); ++i) {
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

    auto fi = functionProtos.find(name);
    if (fi != functionProtos.end())
        return CGFuncProto(fi->second);

    return nullptr;
}

void ObjBuilder::registerCtx(Ptr<ASTContext> ctx)
{
    for (const auto &symListEnt : ctx->getSymbolList()) {
        const auto &symName = symListEnt.first;
        const auto &sym = symListEnt.second;

        switch (sym.symEntTy) {
        case SymEntryType::Unspecified:
        case SymEntryType::All:
        case SymEntryType::Empty:
        case SymEntryType::Function: {
            // TODO
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
            // TODO
            break;
        }
        case SymEntryType::Paramater:
            break;
        }
    }
}

//===----------------------------------------------------------------------===//
// Expressions

llvm::Value *ObjBuilder::CGConstant(Ptr<AST> constantRoot)
{
    // TODO
}

llvm::Value *ObjBuilder::CGConstExpr(Ptr<AST> constExprRoot)
{
    // TODO
}

llvm::Value *ObjBuilder::CGPrimaryExpr(Ptr<AST> varExprRoot)
{
    // TODO
}

llvm::Value *ObjBuilder::CGUnaryExpr(Ptr<AST> unaryExprRoot)
{
    // TODO
}

llvm::Value *ObjBuilder::CGBinaryExpr(Ptr<AST> binaryExprRoot)
{
    // TODO
}

llvm::Value *ObjBuilder::CGTrinaryExpr(Ptr<AST> trinaryExprRoot)
{
    // TODO
}

llvm::Value *ObjBuilder::CGSubscriptExpr(Ptr<AST> subscriptExprRoot)
{
    // TODO
}

llvm::Value *ObjBuilder::CGDerefExpr(Ptr<AST> derefExprRoot)
{
    // TODO
}

llvm::Value *ObjBuilder::CGAddrOfExpr(Ptr<AST> addrOfExprRoot)
{
    // TODO
}

llvm::Value *ObjBuilder::CGAccessExpr(Ptr<AST> accessExprRoot)
{
    // TODO
}

llvm::Value *ObjBuilder::CGCallExpr(Ptr<AST> callExprRoot)
{
    // TODO
}

llvm::Value *ObjBuilder::CGImlicitCastExpr(Ptr<AST> impCastExprRoot)
{
    // TODO
}

llvm::Value *ObjBuilder::CGExplicitCastExpr(Ptr<AST> expCastExprRoot)
{
    // TODO
}

llvm::Value *ObjBuilder::CGExprIDWrapper(Ptr<AST> idWrapperRoot)
{
    auto name = idWrapperRoot->getConstVal<ASTIDType>();
    auto [ty, allocaInst] = findNamedValue(name);
    return theBuilder->CreateLoad(ty, allocaInst, name);
}

llvm::Value *ObjBuilder::CGExprDispatchCN1(Ptr<AST> exprRoot)
{
    auto &child = exprRoot->getChildren()[0];

    switch (child->getSymType()) {

    case ASTSymType::Expr:
        return CGExpr(child);

    case ASTSymType::ExplicitCastExpr:
        return CGExplicitCastExpr(child);

    case ASTSymType::AccessExpr:
        return CGAccessExpr(child);

    case ASTSymType::CallExpr:
        return CGCallExpr(child);

    case ASTSymType::IDWrapper:
        return CGExprIDWrapper(child);

    case ASTSymType::Constant:
        return CGConstant(child);

    case ASTSymType::StringLiteral: {
        // TODO(future)
        splc_ilog_error(&child->getLocation(), false)
            << "using string literals are not supported in LLVMIRGen";
        return nullptr;
    }

    case ASTSymType::SizeOfExpr: {
        // TODO(future)
        splc_ilog_error(&child->getLocation(), false)
            << "sizeof operator is not supported in LLVMIRGen";
        return nullptr;
    }

    default: {
        splc_ilog_error(&exprRoot->getLocation(), true)
            << "unsupported type for dispatch: " << exprRoot->getSymType();
        return nullptr;
    }
    }
}

llvm::Value *ObjBuilder::CGExprDispatchCN2(Ptr<AST> exprRoot)
{
    // TODO
}

llvm::Value *ObjBuilder::CGExprDispatchCN3(Ptr<AST> exprRoot)
{
    // TODO
}

llvm::Value *ObjBuilder::CGExprDispatchCN4(Ptr<AST> exprRoot)
{
    // TODO
}

llvm::Value *ObjBuilder::CGExprDispatchCNN(Ptr<AST> exprRoot)
{
    // TODO
}

llvm::Value *ObjBuilder::CGExpr(Ptr<AST> exprRoot)
{
    splc_dbgassert(exprRoot->isExpr());

    auto N = exprRoot->getChildrenNum();
    switch (N) {
    case 1:
        return CGExprDispatchCN1(exprRoot);
    case 2:
        return CGExprDispatchCN2(exprRoot);
    case 3:
        return CGExprDispatchCN3(exprRoot);
    case 4:
        return CGExprDispatchCN4(exprRoot);
    default:
        return CGExprDispatchCNN(exprRoot);
    }
}

//===----------------------------------------------------------------------===//
// Statements

void ObjBuilder::CGIfStmt(Ptr<AST> ifStmtRoot)
{
    // TODO
}

void ObjBuilder::CGSelStmt(Ptr<AST> selStmtRoot)
{
    // TODO
}

void ObjBuilder::CGDoWhileStmt(Ptr<AST> whileStmtRoot)
{
    // TODO
}
void ObjBuilder::CGWhileStmt(Ptr<AST> whileStmtRoot)
{
    // TODO
}

void ObjBuilder::CGForStmt(Ptr<AST> whileStmtRoot)
{
    // TODO
}

void ObjBuilder::CGIterStmt(Ptr<AST> iterStmtRoot)
{
    // TODO
}

void ObjBuilder::CGCompStmt(Ptr<AST> compStmtRoot)
{
    // TODO
}

void ObjBuilder::CGStmt(Ptr<AST> stmtRoot)
{
    // TODO
}

//===----------------------------------------------------------------------===//
// Declarations

llvm::Function *ObjBuilder::CGFuncProto(Ptr<AST> protoRoot)
{
    // TODO
}

llvm::Function *ObjBuilder::CGFuncDef(Ptr<AST> funcRoot)
{
    // TODO
}

llvm::Value *ObjBuilder::CGInitializer(Ptr<AST> initRoot)
{
    splc_dbgassert(initRoot->isInitializer());
    // TODO(future): support all kinds of initializers

    auto &child = initRoot->getChildren()[0];

    if (!child->isExpr()) {
        splc_ilog_error(&initRoot->getLocation(), true)
            << "non (Assign)Expr node as initializer is not supported";
        return nullptr;
    }

    return CGExpr(child);
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
    theBuilder->CreateStore(initVal, allocaInst);
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

    // Register the context of translation unit
    pushVarCtxStack();
    registerCtx(transUnitRoot->getASTContext());

    auto &child = transUnitRoot->getChildren()[0];
    CGExternDeclList(child);

    // Clean-up
    popVarCtxStack();
}

void ObjBuilder::codegen(TranslationUnit &tunit)
{
    CGTransUnit(tunit.getRootNode());
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
                                  llvm::AllocaInst *allocPos)
{
    auto &varCtx = varCtxStack.back();
    varCtx.namedValues[std::string{name}] = {ty, allocPos};
}

} // namespace splc