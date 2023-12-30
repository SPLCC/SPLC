#ifndef __SPLC_TAC_TACBASE_HH__
#define __SPLC_TAC_TACBASE_HH__ 1

#include "AST/Type.hh"
#include "Core/Base.hh"
#include <cstddef>
#include <string>
#include <vector>

namespace splc {

class TAC;

typedef int ExprType;

extern std::vector<TAC *> tacVector;

}

#endif // __SPLC_TAC_TACBASE_HH__
