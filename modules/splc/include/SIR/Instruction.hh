#ifndef __SPLC_SLR_INSTRUCTION_HH__
#define __SPLC_SLR_INSTRUCTION_HH__ 1

#include "SIR/SIRCommons.hh"
#include "SIR/SymbolTableListTraits.hh"
#include "SIR/User.hh"
#include <list>

namespace splc {

namespace SIR {

class Instruction : public User {
  public:
    //----------------------------------------------------------------------
    // Exported enumerations.
    //
    enum TermOps { // These terminate basic blocks
#define FIRST_TERM_INST(N)              TermOpsBegin = N,
#define HANDLE_TERM_INST(N, OPC, CLASS) OPC = N,
#define LAST_TERM_INST(N)               TermOpsEnd = N + 1
#include "llvm/IR/Instruction.def"
    };

    enum UnaryOps {
#define FIRST_UNARY_INST(N)              UnaryOpsBegin = N,
#define HANDLE_UNARY_INST(N, OPC, CLASS) OPC = N,
#define LAST_UNARY_INST(N)               UnaryOpsEnd = N + 1
#include "llvm/IR/Instruction.def"
    };

    enum BinaryOps {
#define FIRST_BINARY_INST(N)              BinaryOpsBegin = N,
#define HANDLE_BINARY_INST(N, OPC, CLASS) OPC = N,
#define LAST_BINARY_INST(N)               BinaryOpsEnd = N + 1
#include "llvm/IR/Instruction.def"
    };

    enum OtherOps {
#define FIRST_OTHER_INST(N)              OtherOpsBegin = N,
#define HANDLE_OTHER_INST(N, OPC, CLASS) OPC = N,
#define LAST_OTHER_INST(N)               OtherOpsEnd = N + 1
#include "llvm/IR/Instruction.def"
    };

    using InstListType = SymbolTableList<Instruction>;

  public:
    Instruction(const Instruction &) = delete;
    Instruction &operator=(const Instruction &) = delete;

    const Ptr<BasicBlock> getParent() const;
    Ptr<BasicBlock> getParent();
    const Ptr<Module> getModule() const;
    Ptr<Module> getModule();
    const Ptr<Function> getFunction() const;
    Ptr<Function> getFunction();

    /// This method unlinks 'this' from the containing basic block, but does not
    /// delete it.
    void removeFromParent();
    /// This method unlinks 'this' from the containing basic block and deletes
    /// it.
    InstListType::iterator eraseFromParent();

    /// Insert an unlinked instruction into a basic block immediately before the specified instruction.
    void insertBefore(Ptr<Instruction> pos);
    void insertBefore(InstListType::iterator pos);
    void insertAfter(Ptr<Instruction> pos);
    
};

class CallInst : public Instruction {};
class ReturnInst : public Instruction {};
class BranchInst : public Instruction {};
class SwitchInst : public Instruction {};
class CmpInst : public Instruction {
  public:
    enum class Predicate {};
};

} // namespace SIR

} // namespace splc

#endif // __SPLC_SLR_INSTRUCTION_HH__