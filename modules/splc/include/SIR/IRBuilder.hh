#ifndef __SPLC_SIR_IRBUILDER_HH__
#define __SPLC_SIR_IRBUILDER_HH__ 1

#include "AST/ASTCommons.hh"
#include "Core/Base.hh"
#include "SIR/BasicBlock.hh"
#include "SIR/Instruction.hh"
#include "SIR/SIRCommons.hh"

namespace splc::SIR {

class IRBuilderBase {
  protected:
    Ptr<BasicBlock> BB;
    BasicBlock::iterator insertPt;
    SPLCContext &ctxt;

  public:
    IRBuilderBase(SPLCContext &ctxt_) : ctxt(ctxt_) { clearInsertionPoint(); }

    /// Insert and return the specified instruction.
    PtrInst insert(PtrInst inst, const String &name = "");
    PtrVal insert(PtrVal V, const String &name = "");

    //===--------------------------------------------------------------------===//
    // Builder configuration methods
    //===--------------------------------------------------------------------===//
    void clearInsertionPoint();

    Ptr<BasicBlock> getInsertBlock() const;
    BasicBlock::iterator getInsertPoint() const;
    SPLCContext &getContext() const;

    /// This specifies that created instructions should be appended to the
    /// end of the specified block.
    void setInsertPoint(Ptr<BasicBlock> theBB);

    /// This specifies that created instructions should be inserted before
    /// the specified instruction.
    void setInsertPoint(PtrInst I);

    /// This specifies that created instructions should be inserted at
    /// the specified point.
    void setInsertPoint(Ptr<BasicBlock> theBB, BasicBlock::iterator IP);

    /// This specifies that created instructions should be inserted at the
    /// specified point, but also requires that IP is dereferencable.
    void setInsertPoint(BasicBlock::iterator IP);

    PtrVal getTrue();
    PtrVal getFalse();
    PtrVal getInt8(uint8_t C);
    PtrVal getInt16(uint16_t C);
    PtrVal getInt32(uint32_t C);
    PtrVal getInt64(uint64_t C);

    Type *getInt1Ty();
    Type *getInt8Ty();
    Type *getInt16Ty();
    Type *getInt32Ty();
    Type *getInt64Ty();
    Type *getInt128Ty();

    Type *getFloatTy();
    Type *getDoubleTy();
    Type *getVoidTy();

    Ptr<ReturnInst> createRetVoid();
    Ptr<ReturnInst> createRet(PtrVal V);

    /// Create an unconditional 'br label X' instruction.
    Ptr<BranchInst> createBr(Ptr<BasicBlock> dest);

    /// Create a conditional 'br Cond, TrueDest, FalseDest' instruction.
    Ptr<BranchInst> createCondBr(PtrVal cond, Ptr<BasicBlock> t,
                                 Ptr<BasicBlock> f);

    /// Create a switch instruction with the specified value, default dest.
    Ptr<SwitchInst> createSwitch(PtrVal V, Ptr<BasicBlock> dest);

    PtrVal createAdd(PtrVal lhs, PtrVal rhs, const String &name = "");
    PtrVal createSub(PtrVal lhs, PtrVal rhs, const String &name = "");
    PtrVal createMul(PtrVal lhs, PtrVal rhs, const String &name = "");
    PtrVal createUDiv(PtrVal lhs, PtrVal rhs, const String &name = "");
    PtrVal createSDiv(PtrVal lhs, PtrVal rhs, const String &name = "");
    PtrVal createURem(PtrVal lhs, PtrVal rhs, const String &name = "");
    PtrVal createSRem(PtrVal lhs, PtrVal rhs, const String &name = "");
    PtrVal createShl(PtrVal lhs, PtrVal rhs, const String &name = "");
    PtrVal createShr(PtrVal lhs, PtrVal rhs, const String &name = "");
    PtrVal createAShl(PtrVal lhs, PtrVal rhs, const String &name = "");
    PtrVal createAShr(PtrVal lhs, PtrVal rhs, const String &name = "");
    PtrVal createAnd(PtrVal lhs, PtrVal rhs, const String &name = "");
    PtrVal createOr(PtrVal lhs, PtrVal rhs, const String &name = "");
    PtrVal createXor(PtrVal lhs, PtrVal rhs, const String &name = "");

    PtrVal createFAdd(PtrVal lhs, PtrVal rhs, const String &name = "");
    PtrVal createFSub(PtrVal lhs, PtrVal rhs, const String &name = "");
    PtrVal createFMul(PtrVal lhs, PtrVal rhs, const String &name = "");
    PtrVal createFDiv(PtrVal lhs, PtrVal rhs, const String &name = "");
    PtrVal createFRem(PtrVal lhs, PtrVal rhs, const String &name = "");

    PtrVal createBinOp(Instruction::BinaryOps opc, PtrVal lhs, PtrVal rhs,
                       const String &name = "");
    PtrVal createLogicalAnd(PtrVal cond1, PtrVal cond2,
                            const String &name = "");
    PtrVal createLogicalOr(PtrVal cond1, PtrVal cond2, const String &name = "");
    PtrVal createLogicalOp(Instruction::BinaryOps opc, PtrVal cond1,
                           PtrVal cond2, const String &name = "");

    PtrVal createNot(PtrVal V, const String &name = "");
    PtrVal createUnOp(Instruction::UnaryOps opc, PtrVal V,
                      const String &name = "");

    /// Cmp
    PtrVal createCmp(CmpInst::Predicate P, PtrVal lhs, PtrVal rhs,
                     const String &name = "");
    PtrVal createICmp(CmpInst::Predicate P, PtrVal lhs, PtrVal rhs,
                      const String &name = "");
    PtrVal createICmpEQ(PtrVal lhs, PtrVal rhs, const String &name = "");
    PtrVal createICmpNE(PtrVal lhs, PtrVal rhs, const String &name = "");
    PtrVal createICmpUGT(PtrVal lhs, PtrVal rhs, const String &name = "");
    PtrVal createICmpUGE(PtrVal lhs, PtrVal rhs, const String &name = "");
    PtrVal createICmpULT(PtrVal lhs, PtrVal rhs, const String &name = "");
    PtrVal createICmpULE(PtrVal lhs, PtrVal rhs, const String &name = "");
    PtrVal createICmpSGT(PtrVal lhs, PtrVal rhs, const String &name = "");
    PtrVal createICmpSGE(PtrVal lhs, PtrVal rhs, const String &name = "");
    PtrVal createICmpSLT(PtrVal lhs, PtrVal rhs, const String &name = "");
    PtrVal createICmpSLE(PtrVal lhs, PtrVal rhs, const String &name = "");

    PtrVal createFCmp(CmpInst::Predicate P, PtrVal lhs, PtrVal rhs,
                      const String &name = "");
    PtrVal createFCmpEQ(PtrVal lhs, PtrVal rhs, const String &name = "");
    PtrVal createFCmpNE(PtrVal lhs, PtrVal rhs, const String &name = "");
    PtrVal createFCmpUGT(PtrVal lhs, PtrVal rhs, const String &name = "");
    PtrVal createFCmpUGE(PtrVal lhs, PtrVal rhs, const String &name = "");
    PtrVal createFCmpULT(PtrVal lhs, PtrVal rhs, const String &name = "");
    PtrVal createFCmpULE(PtrVal lhs, PtrVal rhs, const String &name = "");
    PtrVal createFCmpSGT(PtrVal lhs, PtrVal rhs, const String &name = "");
    PtrVal createFCmpSGE(PtrVal lhs, PtrVal rhs, const String &name = "");
    PtrVal createFCmpSLT(PtrVal lhs, PtrVal rhs, const String &name = "");
    PtrVal createFCmpSLE(PtrVal lhs, PtrVal rhs, const String &name = "");

    // CallInst
    Ptr<CallInst> createCall(FunctionType *ty, PtrVal callee,
                             Vec<PtrVal> args = {});
};
} // namespace splc::SIR

#endif // __SPLC_SIR_IRBUILDER_HH__