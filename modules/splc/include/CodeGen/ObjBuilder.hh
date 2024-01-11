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

class LLVMDispatchContext {
  public:
    LLVMDispatchContext() = default;
    LLVMDispatchContext(const LLVMDispatchContext &other) = delete;
    LLVMDispatchContext(LLVMDispatchContext &&other) = default;

    std::map<std::string, std::pair<llvm::Type *, llvm::AllocaInst *>,
             std::less<>>
        namedValues;
};

class ObjBuilder {
  public:
    ObjBuilder(SPLCContext &splcCtx_, llvm::LLVMContext &llvmCtx_)
        : splcCtx{splcCtx_}, llvmCtx{llvmCtx_}
    {
        initializeModuleAndManagers();
    }

    void initializeModuleAndManagers();

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

    void registerCtx(Ptr<ASTContext> ctx);

    //===----------------------------------------------------------------------===//
    // Helper Functions

    llvm::Value *CGConstant(Ptr<AST> constantRoot);
    llvm::Value *CGConstExpr(Ptr<AST> constExprRoot);
    llvm::Value *CGPrimaryExpr(Ptr<AST> varExprRoot);
    llvm::Value *CGUnaryExpr(Ptr<AST> unaryExprRoot);
    llvm::Value *CGBinaryExpr(Ptr<AST> binaryExprRoot);
    llvm::Value *CGTrinaryExpr(Ptr<AST> trinaryExprRoot);
    llvm::Value *CGSubscriptExpr(Ptr<AST> subscriptExprRoot);
    llvm::Value *CGDerefExpr(Ptr<AST> derefExprRoot);
    llvm::Value *CGAddrOfExpr(Ptr<AST> addrOfExprRoot);
    llvm::Value *CGAccessExpr(Ptr<AST> accessExprRoot);
    llvm::Value *CGCallExpr(Ptr<AST> callExprRoot);

    llvm::Value *CGImlicitCastExpr(Ptr<AST> impCastExprRoot);
    llvm::Value *CGExplicitCastExpr(Ptr<AST> expCastExprRoot);

    llvm::Value *CGExprIDWrapper(Ptr<AST> idWrapperRoot);

    llvm::Value *CGExprDispatchCN1(Ptr<AST> exprRoot);
    llvm::Value *CGExprDispatchCN2(Ptr<AST> exprRoot);
    llvm::Value *CGExprDispatchCN3(Ptr<AST> exprRoot);
    llvm::Value *CGExprDispatchCN4(Ptr<AST> exprRoot);
    llvm::Value *CGExprDispatchCNN(Ptr<AST> exprRoot);
    llvm::Value *CGExpr(Ptr<AST> exprRoot);

    //===----------------------------------------------------------------------===//
    // Statements

    void CGIfStmt(Ptr<AST> ifStmtRoot);
    void CGSelStmt(Ptr<AST> selStmtRoot);

    void CGDoWhileStmt(Ptr<AST> whileStmtRoot);
    void CGWhileStmt(Ptr<AST> whileStmtRoot);
    void CGForStmt(Ptr<AST> whileStmtRoot);
    void CGIterStmt(Ptr<AST> iterStmtRoot);

    void CGCompStmt(Ptr<AST> compStmtRoot);
    void CGStmt(Ptr<AST> stmtRoot);

    //===----------------------------------------------------------------------===//
    // Declarations

    llvm::Function *CGFuncProto(Ptr<AST> protoRoot);
    llvm::Function *CGFuncDef(Ptr<AST> funcRoot);

    llvm::Value *CGInitializer(Ptr<AST> initRoot);
    void CGInitDecltr(Ptr<AST> initDecltrRoot);
    void CGInitDecltrList(Ptr<AST> initDecltrListRoot);
    void CGDirDecl(Ptr<AST> dirDeclRoot);

    void CGDecl(Ptr<AST> declRoot);

    void CGExternDecl(Ptr<AST> externDeclRoot);

    void CGExternDeclList(Ptr<AST> externDeclListRoot);
    void CGTransUnit(Ptr<AST> transUnitRoot);

    //===----------------------------------------------------------------------===//
    // CG

    void codegen(TranslationUnit &tunit);

    //===----------------------------------------------------------------------===//
    //                             IR/Obj Generation
    //===----------------------------------------------------------------------===//

    void writeLLVMIR();
    void writeProgram();

    //===----------------------------------------------------------------------===//
    //                               Member Access
    //===----------------------------------------------------------------------===//

    llvm::LLVMContext &getLLVMCtx() const noexcept { return llvmCtx; }

    SPLCContext &getSPLCCtx() const noexcept { return splcCtx; }

  protected:
  private:
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
                          llvm::AllocaInst *allocPos);

  private:
    llvm::LLVMContext &llvmCtx;
    SPLCContext &splcCtx;

  private:
    static std::atomic<int> moduleCnt;

    std::map<splc::Type *, llvm::Type *> tyCache;
    Ptr<ASTContext> curASTCtx;

    std::vector<LLVMDispatchContext> varCtxStack;
    std::map<std::string, Ptr<AST>, std::less<>> functionProtos;

    UniquePtr<llvm::Module> theModule;
    UniquePtr<llvm::IRBuilder<>> theBuilder;

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