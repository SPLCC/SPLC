#ifndef __SPLC_SIR_VALUE_HH__
#define __SPLC_SIR_VALUE_HH__ 1

#include "Basic/Type.hh"
#include "SIR/SIRCommons.hh"

namespace splc::SIR {

//===----------------------------------------------------------------------===//
//                                 Value Class
//===----------------------------------------------------------------------===//

/// LLVM Value Representation
///
/// This is a very important LLVM class. It is the base class of all values
/// computed by a program that may be used as operands to other values. Value is
/// the super class of other important classes such as Instruction and Function.
/// All Values have a Type. Type is not a subclass of Value. Some values can
/// have a name and they belong to some Module.  Setting the name on the Value
/// automatically updates the module's symbol table.
///
/// Every value has a "use list" that keeps track of which other Values are
/// using this Value.  A Value can also have an arbitrary number of ValueHandle
/// objects that watch it and listen to RAUW and Destroy events.  See
/// llvm/IR/ValueHandle.h for details.
class Value {
  public:
    enum class ValueTy {};

    Value(const Value &) = delete;
    Value &operator=(const Value&) = delete;

    /// Delete a pointer to a generic Value.
    void deleteValue();

    Type* getType() const;
    SPLCContext &getContext() const;

    bool hasName() const;
    Ptr<String> getName() const;
    void setName(const String &name);
    /// Transfer the name from V to this value.
    void takeName(Ptr<Value> V);

    /// Change all uses of this to point to a new Value.
    void replaceAllUsesWith(Ptr<Value> V);

    // TODO iterator


    unsigned getUseNum() const;
    void addUse(Use &U);

    /// Return an ID for the concrete type of this object.
    unsigned getValueID() const;

    void print(std::ostream os) const;

  protected:
    Value(Type *ty, unsigned scid);
    ~Value();
};
} // namespace splc::SIR

#endif // __SPLC_SIR_VALUE_HH__