#ifndef __SPLC_IR_IR_HH__
#define __SPLC_IR_IR_HH__ 1

#include <Core/splc.hh>

namespace splc {

// TODO: modify everything
struct IR {
    virtual std::string getRepr();
    
};

} // namespace splc

#endif // __SPLC_IR_IR_HH__
