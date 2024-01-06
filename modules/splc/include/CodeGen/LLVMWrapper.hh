#ifndef __SPLC_CODEGEN_LLVMWRAPPER_HH__
#define __SPLC_CODEGEN_LLVMWRAPPER_HH__ 1

#include "AST/DerivedAST.hh"

#include "llvm/ADT/APFloat.h"
#include "llvm/ADT/STLExtras.h"
#include "llvm/IR/BasicBlock.h"
#include "llvm/IR/Constants.h"
#include "llvm/IR/DerivedTypes.h"
#include "llvm/IR/Function.h"
#include "llvm/IR/IRBuilder.h"
#include "llvm/IR/Instructions.h"
#include "llvm/IR/LLVMContext.h"
#include "llvm/IR/Module.h"
#include "llvm/IR/PassManager.h"
#include "llvm/IR/Type.h"
#include "llvm/IR/Verifier.h"
#include "llvm/Passes/PassBuilder.h"
#include "llvm/Passes/StandardInstrumentations.h"
#include "llvm/Support/TargetSelect.h"
#include "llvm/Target/TargetMachine.h"
#include "llvm/Transforms/InstCombine/InstCombine.h"
#include "llvm/Transforms/Scalar.h"
#include "llvm/Transforms/Scalar/GVN.h"
#include "llvm/Transforms/Scalar/Reassociate.h"
#include "llvm/Transforms/Scalar/SimplifyCFG.h"
#include "llvm/Transforms/Utils.h"

namespace splc {

namespace LLVMWrapper {

llvm::Type *cvtIRPrimitiveType(llvm::LLVMContext &C, splc::Type *primTy);

llvm::FunctionType *cvtIRFunctionType(llvm::LLVMContext &C, splc::Type *fTy);

llvm::StructType *cvtIRStructType(llvm::LLVMContext &C, splc::Type *sTy);

llvm::ArrayType *cvtIRArrayType(llvm::LLVMContext &C, splc::Type *arrTy);

llvm::PointerType *cvtIRPointerType(llvm::LLVMContext &C, splc::Type *ptrTy);

} // namespace LLVMWrapper

} // namespace splc

#endif // __SPLC_CODEGEN_LLVMWRAPPER_HH__