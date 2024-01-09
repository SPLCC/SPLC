#ifndef __SPLC_SIR_USER_HH__
#define __SPLC_SIR_USER_HH__ 1

#include "SIR/SIRCommons.hh"
#include "SIR/Value.hh"

namespace splc {
    
namespace SIR {

class User : public Value {
  public:
    static bool classof(const Ptr<Value> V);

    User (const User&) = delete;

    /// Free memory allocated for User and Use objects.
    void operator delete(void *usr);

    const Ptr<Use> getOperandList() const;
    Ptr<Use> getOperandList();
    Ptr<Value> getOperand(unsigned i) const;
    void setOperand(unsigned i, Ptr<Value> val);
    
    const Use& getOperandUse(unsigned i) const;
    Use& getOperandUse(unsigned i);

    unsigned getNumOperands() const;

    // TODO: iterator

  protected:
    void* operator new(size_t size);
    void* operator new(size_t size, unsigned us);

    User(Type* ty, unsigned vTy, Ptr<Use> use, unsigned numOps);
    ~User() = default;
};
} // namespace SIR

} // namespace splc

#endif // __SPLC_SIR_USER_HH__