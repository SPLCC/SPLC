#ifndef __SPLC_AST_TYPECHECK_HH__
#define __SPLC_AST_TYPECHECK_HH__ 1

#include "AST/ASTCommons.hh"
#include "Core/splc.hh"
#include "TypeContext.hh"
#include <iostream>
#include <map>
#include <vector>

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