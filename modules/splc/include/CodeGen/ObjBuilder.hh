#ifndef __SPLC_CODEGEN_LLVMIRGEN_HH__
#define __SPLC_CODEGEN_LLVMIRGEN_HH__ 1

#include "AST/DerivedAST.hh"
#include "CodeGen/ASTDispatch.hh"
#include "CodeGen/LLVMWrapper.hh"
#include "Translation/TranslationUnit.hh"
#include <atomic>

namespace splc {

static constexpr unsigned int defaultAddrSpace = 0;

class ObjBuilderConfig {
    // TODO
};

class ObjParsingContext {
  public:
    ObjParsingContext() = default;
    ObjParsingContext(const ObjParsingContext &other) = delete;
    ObjParsingContext(ObjParsingContext &&other) = default;

    std::map<std::string, std::pair<llvm::Type *, llvm::AllocaInst *>,
             std::less<>>
        namedValues;
};

class ObjBuilder {
  public:
    ObjBuilder() = default;
    ObjBuilder(const ObjBuilder &other) = delete;
    ObjBuilder(ObjBuilder &&other) = default;

    ~ObjBuilder();

  private:
    //===----------------------------------------------------------------------===//
    //                               Type Conversions
    //===----------------------------------------------------------------------===//

    llvm::Type *getPrimitiveType(splc::Type *ty);
    llvm::FunctionType *getFunctionType(splc::FunctionType *ty);
    llvm::StructType *getStructType(splc::StructType *ty);
    llvm::PointerType *getPointerType(splc::PointerType *ty);
    llvm::ArrayType *getArrayType(splc::ArrayType *ty);

    llvm::Type *getCvtType(splc::Type *ty);

    //===----------------------------------------------------------------------===//
    //                               Code Generation
    //===----------------------------------------------------------------------===//

    //===----------------------------------------------------------------------===//
    // AST Parser

    llvm::Function *getFunction(std::string_view name);

    void registerGlobalCtxMutableVar(std::string_view name,
                                     const SymbolEntry &ent);
    void registerCtxMutableVar(std::string_view name, const SymbolEntry &ent);
    // void registerCtxFuncParam(std::string_view name, const SymbolEntry &ent);
    void registerCtxFuncProto(std::string_view name, const SymbolEntry &ent);
    void registerCtxFuncDef(std::string_view name, const SymbolEntry &ent);
    void registerCtx(Ptr<ASTContext> ctx);

    //===----------------------------------------------------------------------===//
    // Helper Functions

    llvm::Value *CGConstant(Ptr<AST> constantRoot);
    // llvm::Value *CGConstExpr(Ptr<AST> constExprRoot);
    llvm::Value *CGPrimaryExpr(Ptr<AST> primaryExprRoot);
    llvm::Value *CGPostfixExpr(Ptr<AST> postfixExprRoot);
    llvm::Value *CGUnaryExpr(Ptr<AST> unaryExprRoot);
    llvm::Value *CGSubscriptExpr(Ptr<AST> subscriptExprRoot);
    llvm::Value *CGDerefExpr(Ptr<AST> derefExprRoot);
    llvm::Value *CGAddrOfExpr(Ptr<AST> addrOfExprRoot);
    llvm::Value *CGAccessExpr(Ptr<AST> accessExprRoot);
    llvm::Value *CGCallExpr(Ptr<AST> callExprRoot);
    llvm::Value *CGSizeOfExpr(Ptr<AST> sizeOfExprRoot);
    llvm::Value *CGAssignExpr(Ptr<AST> assignExprRoot);

    llvm::Value *CGBinaryCondExpr(Ptr<AST> binCondExprRoot);
    llvm::Value *CGBinaryArithExpr(Ptr<AST> binaryExprRoot);
    llvm::Value *CGTrinaryCondExpr(Ptr<AST> triCondExprRoot);

    llvm::Value *CGImplicitCastExpr(Ptr<AST> impCastExprRoot);
    llvm::Value *CGExplicitCastExpr(Ptr<AST> expCastExprRoot);

    llvm::Value *CGExprID(Ptr<AST> IDRoot);

    llvm::Value *CGGeneralExprDispCN1(Ptr<AST> exprRoot);
    llvm::Value *CGGeneralExprDispCN2(Ptr<AST> exprRoot);
    llvm::Value *CGGeneralExprDispCN3(Ptr<AST> exprRoot);
    llvm::Value *CGGeneralExprDispCNN(Ptr<AST> exprRoot);
    llvm::Value *CGGeneralExprDispatch(Ptr<AST> exprRoot);

    //===----------------------------------------------------------------------===//
    // Statements

    void CGCompStmt(Ptr<AST> compStmtRoot);

    void CGExprStmt(Ptr<AST> exprStmtRoot);

    void CGIfStmt(Ptr<AST> ifStmtRoot);
    void CGSelStmt(Ptr<AST> selStmtRoot);

    void CGDoWhileStmt(Ptr<AST> doWhileStmtRoot);
    void CGWhileStmt(Ptr<AST> whileStmtRoot);
    void CGForStmt(Ptr<AST> forStmtRoot);
    void CGIterStmt(Ptr<AST> iterStmtRoot);

    void CGLabeledStmt(Ptr<AST> labeledStmtRoot);

    void CGReturnStmt(Ptr<AST> returnStmtRoot);
    void CGJumpStmt(Ptr<AST> jumpStmtRoot);

    void CGStmt(Ptr<AST> stmtRoot);

    //===----------------------------------------------------------------------===//
    // Declarations

    /// Accept FuncDecltr as protoRoot.
    llvm::Function *CGFuncProto(Ptr<AST> protoRoot);
    /// Accept FuncDef as funcRoot.
    llvm::Function *CGFuncDef(Ptr<AST> funcRoot);

    llvm::Value *CGInitializer(Ptr<AST> initRoot);
    void CGInitDecltr(Ptr<AST> initDecltrRoot);
    void CGInitDecltrList(Ptr<AST> initDecltrListRoot);
    void CGDirDecl(Ptr<AST> dirDeclRoot);

    void CGDecl(Ptr<AST> declRoot);

    void CGExternDecl(Ptr<AST> externDeclRoot);

    void CGExternDeclList(Ptr<AST> externDeclListRoot);
    void CGTransUnit(Ptr<AST> transUnitRoot);

  public:
    //===----------------------------------------------------------------------===//
    // CG

    void generateModule(TranslationUnit &tunit);
    void optimizeModule();

    //===----------------------------------------------------------------------===//
    //                             IR/Obj Generation
    //===----------------------------------------------------------------------===//

    void writeModuleAsLLVMIR(std::ostream &os);
    void writeModuleAsAsm(
        std::string_view path,
        std::string_view targetTriple = llvm::sys::getDefaultTargetTriple());
    void writeModuleAsObj(
        std::string_view path,
        std::string_view targetTriple = llvm::sys::getDefaultTargetTriple());

    //===----------------------------------------------------------------------===//
    //                               Member Access
    //===----------------------------------------------------------------------===//

    llvm::LLVMContext &getLLVMCtx() const noexcept { return *llvmCtx; }

  protected:
  private:
    //===----------------------------------------------------------------------===//
    //                          IR/Obj Generation Impl
    //===----------------------------------------------------------------------===//

    void generateModuleImpl(TranslationUnit &tunit);
    void optimizeModuleImpl();
    void writeModuleLLVMIRImpl(std::ostream &os);
    void writeModuleAsFile(llvm::CodeGenFileType fileType,
                           std::string_view path,
                           std::string_view targetTriple);

    //===----------------------------------------------------------------------===//
    //                          Internal State Management
    //===----------------------------------------------------------------------===//

    void initializeModuleAndManagers();
    void initializeTargetRegistry();

    void initializeInternalStates();

    void setGenerationStatus(bool s) noexcept
    {
        llvmModuleGenerationSuccess = s;
    }

    bool isGenerationSuccess() const noexcept
    {
        return llvmModuleGenerationSuccess;
    }

    //===----------------------------------------------------------------------===//
    // Variable Management

    llvm::AllocaInst *createEntryBlockAlloc(llvm::Function *theFunction,
                                            llvm::Type *type,
                                            llvm::Value *arrSize = nullptr,
                                            std::string_view varName = "");

    void pushVarCtxStack();
    void popVarCtxStack();
    size_t varCtxStackSize() const noexcept { return varCtxStack.size(); }
    bool isVarCtxGlobalScope() const noexcept { return varCtxStackSize() == 1; }

    /// Find named value starting from the top of the stack
    std::pair<llvm::Type *, llvm::AllocaInst *>
    findNamedValue(std::string_view name) const;
    void insertNamedValue(std::string_view name, llvm::Type *ty,
                          llvm::AllocaInst *alloca);

    void registerFuncProto(std::string_view name, llvm::Type *ty,
                           Ptr<AST> protoRoot);
    std::pair<llvm::Type *, Ptr<AST>> findFuncProto(std::string_view name);

  private:
    UniquePtr<llvm::LLVMContext> llvmCtx;

    bool llvmTargetEnvInitialized = false;
    bool llvmModuleGenerated = false;
    bool llvmModuleGenerationSuccess = false;

  private:
    static std::atomic<int> moduleCnt;

    std::map<splc::Type *, llvm::Type *> tyCache;

    std::vector<ObjParsingContext> varCtxStack;
    std::map<std::string, std::pair<llvm::Type *, Ptr<AST>>, std::less<>>
        functionProtos;

    UniquePtr<llvm::Module> theModule;
    UniquePtr<llvm::IRBuilder<>> builder;

    UniquePtr<llvm::FunctionPassManager> theFPM;
    UniquePtr<llvm::LoopAnalysisManager> theLAM;
    UniquePtr<llvm::FunctionAnalysisManager> theFAM;
    UniquePtr<llvm::CGSCCAnalysisManager> theCGAM;
    UniquePtr<llvm::ModuleAnalysisManager> theMAM;
    UniquePtr<llvm::PassInstrumentationCallbacks> thePIC;
    UniquePtr<llvm::StandardInstrumentations> theSI;
};

} // namespace splc

#endif // __SPLC_CODEGEN_LLVMIRGEN_HH__