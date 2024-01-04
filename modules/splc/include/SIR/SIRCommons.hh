#ifndef __SPLC_SIR_SIRCOMMONS_HH__
#define __SPLC_SIR_SIRCOMMONS_HH__ 1
#include <string>
#include <vector>
#include "Core/Base.hh"

namespace splc {

namespace SIR {

class Instruction;
class User;
class Value;

class BasicBlock;
class Instruction;
class IRBuilderBase;

class Module;
class Function;
class Argument;

class IRBuilderBase;

using String = std::string;

using PtrBB = Ptr<BasicBlock>;
using PtrArg = Ptr<Argument>;
using PtrFunc = Ptr<Function>;

template <class T>
using Vec = std::vector<T>;


}
} // namespace splc

#endif // __SPLC_SIR_SIRCOMMONS_HH__