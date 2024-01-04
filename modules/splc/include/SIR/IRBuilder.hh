#ifndef __SPLC_SIR_IRBUILDER_HH__
#define __SPLC_SIR_IRBUILDER_HH__ 1

#include "Core/Base.hh"
#include "AST/ASTCommons.hh"
#include "SIR/SIRCommons.hh"
#include "SIR/BasicBlock.hh"
#include "SIR/Instruction.hh"

namespace splc {

namespace SIR {



class IRBuilderBase {
  protected:
    PtrBB BB;
    BasicBlock::iterator insertPt;
    SPLCContext &ctxt;

  public:
    IRBuilderBase(SPLCContext &ctxt_) : ctxt(ctxt_) { clearInsertionPoint(); }

    /// Insert and return the specified instruction.
    Ptr<Instruction> insert(Ptr<Instruction> inst, const String &name = "") {
        if (BB) {
            inst->insertInto(BB, insertPt);
        }
        // TODO
        // inst.setName(Name);
        return inst;
    }

    //===--------------------------------------------------------------------===//
    // Builder configuration methods
    //===--------------------------------------------------------------------===//
    void clearInsertionPoint()
    {
        BB = nullptr;
        insertPt = BasicBlock::iterator();
    }

    PtrBB getInsertBlock() const { return BB; }
    BasicBlock::iterator getInsertPoint() const { return insertPt; };
    SPLCContext &getContext() const {return ctxt;};

    /// This specifies that created instructions should be appended to the
    /// end of the specified block.
    void setInsertPoint(PtrBB theBB) {
        BB = theBB;
        insertPt = BB->end();
    }


};

} // namespace SIR

} // namespace splc

#endif // __SPLC_SIR_IRBUILDER_HH__