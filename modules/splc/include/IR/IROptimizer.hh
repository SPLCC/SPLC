#ifndef __SPLC_IR_IROPTIMIZER_HH__
#define __SPLC_IR_IROPTIMIZER_HH__ 1

#include <AST/AST.hh>
#include <Core/splc.hh>
#include <IR/IRBuilder.hh>

namespace splc {

class IROptimizer;

class IROptimizer {
  public:
    void optimize(splc::IRBuilder &irBuilder);

    void constantFold(splc::IRBuilder &irBuilder);

    void deadCodeElimination(IRBuilder &irBuilder);
};

} // namespace splc

#endif // __SPLC_IR_IROPTIMIZER_HH__
