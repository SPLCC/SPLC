#ifndef __SPLC_AST_VALUE_HH__
#define __SPLC_AST_VALUE_HH__ 1

#include <memory>

#include "Core/splc.hh"

#include "AST/ASTCommons.hh"
#include "AST/Type.hh"

namespace splc {

/// 
/// \brief `Value` represents the result of evaluation on AST.
/// 
class Value : public std::enable_shared_from_this<Value> {
  public:
    // TODO
    Value() = delete;
    Value(Type type_) : type{type_} {}

    Type type;
    ASTValueType val; ///< Actual value, if any, through const propagation
    bool isLValue;
    bool isRValue;
};

} // namespace splc

#endif // __SPLC_AST_VALUE_HH__