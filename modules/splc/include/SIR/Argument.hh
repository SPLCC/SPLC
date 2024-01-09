#ifndef __SPLC_SIR_ARGUMENT_HH__
#define __SPLC_SIR_ARGUMENT_HH__ 1
#include "SIR/SIRCommons.hh"
#include "SIR/Value.hh"

namespace splc::SIR {

class Argument final : public Value {
  public:
    Argument(Type *ty, const String &name = "", Ptr<Function> = nullptr,
             unsigned argNo = 0);
    
    const Ptr<Function> getParent() const;
    Ptr<Function> getParent();
    unsigned getArgNo() const;

    Type *getType() const;

    static bool classof(const Ptr<Value> V);

    friend class Function;

};
} // namespace splc::SIR

#endif // __SPLC_SIR_ARGUMENT_HH__