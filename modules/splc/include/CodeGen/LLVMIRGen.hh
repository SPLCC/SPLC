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

    llvm::LLVMContext &getLLVMCtx() const noexcept { return llvmCtx; }

    SPLCContext &getSPLCCtx() const noexcept { return splcCtx; }

  protected:
  private:
    llvm::LLVMContext &llvmCtx;
    SPLCContext &splcCtx;

    std::map<splc::Type *, llvm::Type *> tyCache;
    std::map<llvm::Type *, splc::Type *> revTyCache;
};

} // namespace splc

#endif // __SPLC_CODEGEN_LLVMIRGEN_HH__