#ifndef __SPLC_SIR_BASICBLOCK_HH__
#define __SPLC_SIR_BASICBLOCK_HH__ 1

#include "SIR/SIRCommons.hh"
#include "SIR/Value.hh"
#include <list>

namespace splc {

namespace SIR {

class BasicBlock final : public Value {
  public:
    using InstListType = std::list<Ptr<Instruction>>;
    using iterator = InstListType::iterator;
    using const_iterator = InstListType::const_iterator;

    iterator begin() { return instructions.begin(); }
    iterator end() { return instructions.end(); }
    const_iterator begin() const { return instructions.begin(); }
    const_iterator end() const { return instructions.end(); }

  private:
    InstListType instructions;
};

} // namespace SIR

} // namespace splc

#endif // __SPLC_SIR_BASICBLOCK_HH__