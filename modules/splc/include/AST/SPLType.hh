#ifndef __SPLC_AST_SPLTYPE_HH__
#define __SPLC_AST_SPLTYPE_HH__ 1

#include "AST/ASTCommons.hh"
#include "Basic/SPLCContext.hh"
#include "Core/splc.hh"
#include <iostream>
#include <map>
#include <vector>

namespace splc {

class SPLType {
  public:
    SPLType(Type *const ty_, bool isConst_ = false, bool isRestrict_ = false,
            bool isVolatile_ = false, bool isAtomic_ = false,
            bool isInline_ = false)
        : ty{ty_}, isConst{isConst_}, isRestrict{isRestrict_},
          isVolatile{isVolatile_}, isAtomic{isAtomic_}, isInline(isInline_)
    {
    }

    const bool isConst;
    const bool isRestrict;
    const bool isVolatile;
    const bool isAtomic;
    const bool isInline;
    Type *const ty;
};

} // namespace splc

#endif // __SPLC_AST_SPLTYPE_HH__