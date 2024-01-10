#ifndef __SPLC_CODEGEN_LLVMIRGEN_HH__
#define __SPLC_CODEGEN_LLVMIRGEN_HH__ 1

#include "AST/DerivedAST.hh"
#include "CodeGen/ASTDispatch.hh"
#include "CodeGen/LLVMWrapper.hh"
#include "Translation/TranslationUnit.hh"

namespace splc {

constexpr unsigned int defaultAddrSpace = 0;

class LLVMIRBuilder {
  public:
    LLVMIRBuilder(SPLCContext &splcCtx_, llvm::LLVMContext &llvmCtx_)
        : splcCtx{splcCtx_}, llvmCtx{llvmCtx_}
    {
    }

    llvm::Type *getPrimitiveType(splc::Type *ty);
    llvm::FunctionType *getFunctionType(splc::FunctionType *ty);
    llvm::StructType *getStructType(splc::StructType *ty);
    llvm::PointerType *getPointerType(splc::PointerType *ty);
    llvm::ArrayType *getArrayType(splc::ArrayType *ty);

    llvm::Type *getCvtType(splc::Type *ty);

    void codegen(TranslationUnit &tunit);

    //===----------------------------------------------------------------------===//
    //                               Member Access

    llvm::LLVMContext &getLLVMCtx() const noexcept { return llvmCtx; }

    SPLCContext &getSPLCCtx() const noexcept { return splcCtx; }

  protected:
  private:
    llvm::LLVMContext &llvmCtx;
    SPLCContext &splcCtx;

  private:
    std::map<splc::Type *, llvm::Type *> tyCache;
    Ptr<ASTContext> curASTCtx;

    std::map<std::string, llvm::AllocaInst *> namedValues;
    std::map<std::string, Ptr<AST>> functionProtos;

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