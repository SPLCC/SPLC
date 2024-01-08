#ifndef __SPLC_CODEGEN_LLVMIRGEN_HH__
#define __SPLC_CODEGEN_LLVMIRGEN_HH__ 1

#include "AST/DerivedAST.hh"
#include "CodeGen/ASTDispatch.hh"
#include "CodeGen/LLVMWrapper.hh"
#include "Translation/TranslationUnit.hh"

namespace splc {

class LLVMIRBuilder {
  public:
    LLVMIRBuilder() = default;

    void codegen(Ptr<TranslationUnit> tunit);

  private:
};

} // namespace splc

#endif // __SPLC_CODEGEN_LLVMIRGEN_HH__