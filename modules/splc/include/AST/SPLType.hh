#ifndef __SPLC_AST_SPLTYPE_HH__
#define __SPLC_AST_SPLTYPE_HH__ 1

#include "AST/ASTCommons.hh"
#include "Basic/TypeContext.hh"
#include "Core/splc.hh"
#include <iostream>
#include <map>
#include <vector>

namespace splc {

class SPLType {
  public:
    const bool isConst;
    const bool isRestrict;
    const bool isVolatile;
    const bool isAtomic;
    const bool isInline;
    Type *containedTy;
};

} // namespace splc

#endif // __SPLC_AST_SPLTYPE_HH__