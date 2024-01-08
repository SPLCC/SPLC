#ifndef __SPLC_SIR_FUNCTION_HH__
#define __SPLC_SIR_FUNCTION_HH__ 1

#include "Basic/SPLCContext.hh"
#include "Core/Base.hh"
#include "SIR/SIRCommons.hh"
#include <string>
#include <vector>

namespace splc::SIR {

class Function {
  public:
    using BasicBlockListType = Vec<BasicBlock>;
    using iterator = BasicBlockListType::iterator;
    using const_iterator = BasicBlockListType::const_iterator;
    using arg_iterator = Ptr<Argument>;
    using const_arg_iterator = const Ptr<Argument>;

    Function(const Function &) = delete;
    void operator=(const Function &) = delete;
    ~Function() = default;
    
    static Ptr<Function> create(FunctionType *ty, const String &name = "", Module *M = nullptr);
    static Ptr<Function> create(FunctionType *ty, const String &name, Module &M);
    static bool classof(const Ptr<Value> V);

    /// Returns the number of non-debug IR instructions in this function.
    unsigned getInstructionCount() const;

    FunctionType *getFunctionType() const;
    Type *getReturnType() const;
    SPLCContext &getContext() const;

    /// Return true if this function takes a variable number of arguments.
    bool isVarArg() const;

    /// Extract the byval type for a parameter.
    Type *getParamByValType(unsigned argNo) const;
    /// Extract the byref type for a parameter.
    Type *getParamByRefType(unsigned argNo) const;

    /// Determine if the function cannot return.
    bool doesNotReturn() const;
    void setDoesNotReturn() const;
    /// Determine if the function will return.
    bool willReturn() const;
    void setWillReturn() const;
    /// Determine if the function is known not to recurse, directly or
    /// indirectly.
    bool doesNotRecurse() const;
    void setDoesNotRecurse();

    /// This method deletes the body of the function, and converts the linkage
    /// to external.
    void deleteBody();
    /// This method unlinks 'this' from the containing module, but does not
    /// delete it.
    void removeFromParent();
    /// This method unlinks 'this' from the containing module and deletes it.
    void eraseFromParent();

    /// Insert \p BB in the basic block list at \p pos.
    Function::iterator insert(Function::iterator pos, Ptr<BasicBlock> BB);

    /// Transfer all blocks from \p fromF to this function at \p toIt.
    void splice(Function::iterator toIt, Ptr<Function> fromF);
    /// Transfer one BasicBlock from \p fromF at \p fromIt to this function at
    /// \p toIt.
    void splice(Function::iterator toIt, Ptr<Function> fromF,
                Function::iterator fromIt);

    /// Transfer a range of basic blocks that belong to \p fromF from \p
    /// fromBeginIt to \p fromEndIt, to this function at \p toIt.
    void splice(Function::iterator toIt, Ptr<Function> fromF,
                Function::iterator fromBeginIt, Function::iterator fromEndIt);

    /// Erases a range of BasicBlocks from \p fromIt to (not including) \p toIt.
    Function::iterator erase(Function::iterator fromIt,
                             Function::iterator toIt);

    const BasicBlock &getEntryBlock() const;
    BasicBlock &getEntryBlock();

    const Ptr<ValueSymbolTable> getValueSymbolTable() const;
    Ptr<ValueSymbolTable> getValueSymbolTable();

    iterator begin();
    const_iterator begin() const;
    iterator end();
    const_iterator end() const;

    /// Return the size of BasicBlocks.
    size_t size() const;
    bool empty() const;

    const BasicBlock &front() const;
    BasicBlock &front();
    const BasicBlock &back() const;
    BasicBlock &back();

    size_t arg_size() const;
    bool arg_empty() const;
    arg_iterator arg_begin();

    const_arg_iterator arg_begin() const;
    arg_iterator arg_end();
    const_arg_iterator arg_end() const;
    Ptr<Argument> getArg(unsigned i) const;
};

} // namespace splc::SIR

#endif // __SPLC_SIR_FUNCTION_HH__