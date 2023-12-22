#ifndef __SPLC_AST_TYPECHECK_HH__
#define __SPLC_AST_TYPECHECK_HH__ 1

#include <iostream>
#include <map>
#include <vector>

#include "Core/splc.hh"

#include "AST/ASTCommons.hh"
#include "TypeContext.hh"

namespace splc {

enum class TypeCastResult {
    Widening,
    Equivalent,
    Narrowing,
    Fail,
    WideningNEQCV,
    NEQCV,
    NarrowingNEQCV,
};

// TODO:
static TypeCastResult tryCast(Type *t1, Type *t2);

} // namespace splc

#endif // __SPLC_AST_TYPECHECK_HH__