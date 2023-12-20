#ifndef __SPLC_CODEGEN_SIMINSTGEN_HH__
#define __SPLC_CODEGEN_SIMINSTGEN_HH__ 1

#include <string>

#include "AST/ASTNode.hh"

#include "CodeGen/Builder.hh"

namespace splc::codegen {

class SimBuilder : public Builder {
  public:
    SimBuilder() = default;

  private:
};

} // namespace splc::codegen

#endif // __SPLC_CODEGEN_SIMINSTGEN_HH__