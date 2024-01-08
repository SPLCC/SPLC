#ifndef __SPLC_SIR_BASICBLOCK_HH__
#define __SPLC_SIR_BASICBLOCK_HH__ 1

#include "SIR/SIRCommons.hh"
#include "SIR/Value.hh"
#include <list>

namespace splc::SIR {

/// SPLC Basic Block Representation
///
/// This represents a single basic block in SPLC. A basic block is simply a
/// container of instructions that execute sequentially. Basic blocks are Values
/// because they are referenced by instructions such as branches and switch
/// tables. The type of a BasicBlock is "Type::LabelTy" because the basic block
/// represents a label to which a branch can jump.
///
/// A well formed basic block is formed of a list of non-terminating
/// instructions followed by a single terminator instruction. Terminator
/// instructions may not occur in the middle of basic blocks, and must terminate
/// the blocks. The BasicBlock class allows malformed basic blocks to occur
/// because it may be useful in the intermediate stage of constructing or
/// modifying a program. However, the verifier will ensure that basic blocks are
/// "well formed".
class BasicBlock final : public Value {
  public:
    using InstListType = std::list<Ptr<Instruction>>;
    using iterator = InstListType::iterator;
    using const_iterator = InstListType::const_iterator;
    using reverse_iterator = InstListType::reverse_iterator;
    using const_reverse_iterator = InstListType::const_reverse_iterator;

    BasicBlock(const BasicBlock &) = delete;
    BasicBlock &operator=(const BasicBlock &) = delete;
    ~BasicBlock() = default;

    static Ptr<BasicBlock> create(SPLCContext &ctxt, const String &name = "",
                                  Ptr<Function> parent = nullptr,
                                  Ptr<BasicBlock> insertBefore = nullptr);

    /// 	Methods for support type inquiry through isa, cast, and dyn_cast.
    static bool classof(const Ptr<Value> V);

    SPLCContext &getContext() const;

    /// Return the enclosing method, or null if none.
    const Ptr<Function> getParent() const;
    Ptr<Function> getParent();

    /// Return the module owning the function this basic block belongs to, or
    /// nullptr if the function does not have a module.
    const Ptr<Module> getModule() const;
    Ptr<Module> getModule();

    /// Returns the terminator instruction if the block is well formed or null
    /// if the block is not well formed.
    const Ptr<Instruction> getTerminator() const;
    Ptr<Instruction> getTerminator();

    /// Returns an iterator to the first instruction in this block that
    /// is suitable for inserting a non-PHI instruction.
    const_iterator getFirstInsertionPt() const;
    iterator getFirstInsertionPt();

    /// Unlink this basic block from its current function and insert it into the
    /// function that MovePos lives in, right before MovePos.
    void moveBefore(Ptr<BasicBlock> movePos);
    /// Unlink this basic block from its current function and insert it right
    /// after MovePos in the function MovePos lives in.
    void moveAfter(Ptr<BasicBlock> movePos);

    /// Insert unlinked basic block into a function.
    void insertInto(Ptr<Function> parent,
                    Ptr<BasicBlock> insertBefore = nullptr);

    void print(std::ostream &os);

    /// Instruction iterator methods.
    iterator begin();
    const_iterator begin() const;
    iterator end();
    const_iterator end() const;
    reverse_iterator rbegin();
    const_reverse_iterator rbegin() const;
    reverse_iterator rend();
    const_reverse_iterator rend() const;

    /// Returns a pointer to the symbol table if one exists.
    Ptr<ValueSymbolTable> getValueSymbolTable();

    /// Split the basic block into two basic blocks at the specified
    /// instruction.
    Ptr<BasicBlock> splitBasicBlock(iterator I, const String &name = "",
                                    bool before = false);
    Ptr<BasicBlock> splitBasicBlock(Ptr<Instruction> I, const String &name = "",
                                    bool before = false);

    /// Split the basic block into two basic blocks at the specified instruction
    /// and insert the new basic blocks as the predecessor of the current block.
    Ptr<BasicBlock> splitBasicBlockBefore(iterator I, const String &name = "");
    Ptr<BasicBlock> splitBasicBlockBefore(Ptr<Instruction> I,
                                          const String &name = "");

    /// Transfer one instruction from \p fromBB to this basic block
    /// at \p toIt.
    void splice(BasicBlock::iterator toIt, Ptr<BasicBlock> fromBB);

    /// Transfer one instruction from \p fromBB at \p fromIt to this basic block
    /// at \p toIt.
    void splice(BasicBlock::iterator toIt, BasicBlock *fromBB,
                BasicBlock::iterator fromIt);

    /// Transfer a range of instructions that belong to \p fromBB from \p
    /// FromBeginIt to \p FromEndIt, to this basic block at \p toIt.
    void splice(BasicBlock::iterator toIt, BasicBlock *fromBB,
                BasicBlock::iterator fromBeginIt,
                BasicBlock::iterator fromEndIt);

    /// Erases a range of instructions from \p fromIt to (not including) \p
    /// toIt.
    void erase(BasicBlock::iterator fromIt, BasicBlock::iterator toIt);

  private:
    InstListType instructions;
    Ptr<Function> parent;
};

} // namespace splc::SIR

#endif // __SPLC_SIR_BASICBLOCK_HH__