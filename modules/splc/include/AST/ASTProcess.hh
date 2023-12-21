#ifndef __SPLC_AST_ASTPROCESS_HH__
#define __SPLC_AST_ASTPROCESS_HH__ 1

#include <iostream>
#include <map>
#include <vector>

#include "Core/splc.hh"

#include "AST/AST.hh"

namespace splc {

class ASTProcessor {
  public:
    static AST &removeASTPunctuators(AST &node);
    static AST &reduce(AST &node);
};

} // namespace splc

#endif // __SPLC_AST_ASTPROCESS_HH__