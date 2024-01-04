#ifndef __SPLC_SIR_VALUE_HH__
#define __SPLC_SIR_VALUE_HH__ 1

#include "Basic/Type.hh"
#include "SIR/SIRCommons.hh"

namespace splc {

namespace SIR {

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
  protected:
    Type *vTy;
    
    const unsigned subClassID;  // Subclass identifier (for isa/dyn_cast)
    
  public:
    enum ValueTy {
        ArgumentVal,
    };

    Value(Type *Ty, unsigned scid) : vTy(Ty), subClassID(scid) {}

    virtual ~Value() {}

    Type *getType() const {return vTy;}

    unsigned getValueID() const {return subClassID;}

    String getName() const;

    void setName(const String &name);
};
} // namespace SIR

} // namespace splc

#endif // __SPLC_SIR_VALUE_HH__