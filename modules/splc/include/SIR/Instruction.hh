#ifndef __SPLC_SLR_INSTRUCTION_HH__
#define __SPLC_SLR_INSTRUCTION_HH__ 1

#include "SIR/SIRCommons.hh"
#include "SIR/User.hh"
#include <list>

namespace splc {

namespace SIR {

class Instruction : public User {

  private:
    PtrBB parent;

  public:
    using InstListType = std::list<Ptr<Instruction>>;

    inline const PtrBB getParent() const { return parent; }
    inline PtrBB getParent() { return parent; }
    /// Inserts an unlinked instruction into \p ParentBB at position \p It and
    /// returns the iterator of the inserted instruction.
    InstListType::iterator insertInto(PtrBB parentBB,
                                      InstListType::iterator it);
};
} // namespace SIR

} // namespace splc

#endif // __SPLC_SLR_INSTRUCTION_HH__