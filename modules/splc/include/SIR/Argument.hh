#ifndef __SPLC_SIR_ARGUMENT_HH__
#define __SPLC_SIR_ARGUMENT_HH__ 1
#include "SIR/SIRCommons.hh"
#include "SIR/Value.hh"

namespace splc {

namespace SIR {

class Argument final : public Value {
    Ptr<Function> parent;
    unsigned argNo;

  public:
    Argument(Type *ty, const String &name = "", Ptr<Function> parent = nullptr,
             unsigned argNo = 0)
        : Value(ty, Value::ArgumentVal), parent(parent), argNo(argNo)
    {
        this->setName(name);
    }

    Ptr<Function> getParent() { return parent; }
    const Ptr<Function> getParent() const { return parent; }

    unsigned getArgNo() const { return argNo; }

    Type *getType() { return vTy; }
    const Type* getType() const {return vTy;}

    /// Support for type inquiry through isa, cast, and dyn_cast
    static bool classof(const Ptr<Value> v) {
      return v->getValueID() == Value::ArgumentVal;
    }


};

} // namespace SIR
} // namespace splc

#endif // __SPLC_SIR_ARGUMENT_HH__