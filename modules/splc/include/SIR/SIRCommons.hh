#ifndef __SPLC_SIR_SIRCOMMONS_HH__
#define __SPLC_SIR_SIRCOMMONS_HH__ 1
#include <string>
#include <vector>
#include <list>
#include "Core/Base.hh"

namespace splc {

namespace SIR {

template <class T, typename... args>
class SymbolTableList;

class Value;
class Use;
class User;
class Instruction;
class Constant;
class GlobalValue;
class GlobalVariable;

class BasicBlock;
class Instruction;
class IRBuilderBase;

class Module;
class Function;
class Argument;


class IRBuilderBase;

class ValueSymbolTable;

using String = std::string;

using PtrBB = Ptr<BasicBlock>;
using PtrArg = Ptr<Argument>;
using PtrFunc = Ptr<Function>;
using PtrInst = Ptr<Instruction>;
using PtrVal = Ptr<Value>;

template <class T>
using Vec = std::vector<T>;

template <class T>
using List = std::list<T>;

template<class T>
concept IsBaseInst = (std::is_base_of_v<Instruction, std::remove_cv_t<T>>);

}
} // namespace splc

#endif // __SPLC_SIR_SIRCOMMONS_HH__