#ifndef __SPLC_IR_IR_HH__
#define __SPLC_IR_IR_HH__ 1

#include <AST/AST.hh>
#include <Core/splc.hh>

namespace splc {

using IRIDType = ASTIDType;
using StrRef = std::string_view;

template <class T>
using IRVec = std::vector<T>;

template <class T, class U>
using IRMap = std::map<T, U, std::less<>>;

template <class T, class U>
using IRPair = std::pair<T, U>;

template <class T>
using IRSet = std::set<T>;

enum class IRVarType {
    Label,
    Function,
    Variable,
    Constant,
};

enum class IRType {
    SetLabel,
    FuncDecl,
    Assign,
    Plus,
    Minus,
    Mul,
    Div,
    AddrOf,
    Deref,
    CopyToAddr,
    Goto,
    BranchIf,
    Return,
    Alloc,
    PopCallArg,
    PushCallArg,
    InvokeFunc,
    Read,
    Write,
};

enum class IRBranchType {
    None,
    LT,
    LE,
    GT,
    GE,
    EQ,
    NE,
};

class IRVar;
class IRStmt;
class IRFunction;

class IRVar {
  public:
    IRVar(IRIDType name_, IRVarType varType_ = IRVarType::Label,
          Type *type_ = nullptr, ASTValueType val_ = {}, bool isConst_ = false)
        : name{name_}, irVarType{varType_}, valType{type_}, val{val_},
          isConst{isConst_}
    {
    }

    constexpr bool hasValue() const noexcept { return val.index() != 0; }

    template <IsValidASTValue T>
    auto getValue()
    {
        return std::get<T>(val);
    }

    template <class Visitor>
    auto visitValue(Visitor &&vis) const
    {
        return std::visit(vis, val);
    }

    template <IsValidASTValue T>
    void emplaceValue(T &&val_)
    {
        val.emplace<T>(std::forward(val_));
    }

    template <IsValidASTValue T>
    constexpr bool holdsValueType() const noexcept
    {
        return std::holds_alternative<T>(val);
    }

    // ASSUMPTION: NO SEMANTIC ERROR
    // ASSUMPTION: NO RVALUE ASSIGNMENT, THUS CONST PROPAGATION
    IRIDType name;
    IRVarType irVarType;
    Type *valType;
    ASTValueType val;
    bool isConst;
};

// TODO: modify everything
class IRStmt {
  public:
    IRStmt(IRType irType_, Ptr<IRVar> op1_, Ptr<IRVar> op2_ = nullptr,
           Ptr<IRVar> op3_ = nullptr,
           IRBranchType branchType_ = IRBranchType::None)
        : irType{irType_}, op1{op1_}, op2{op2_}, op3{op3_},
          branchType{branchType_}
    {
    }

    IRStmt(IRType irType_, Ptr<IRFunction> func, Ptr<IRVar> lhs_)
        : irType{irType_}, target{func.get()}, op1{lhs_}
    {
    }

    IRType irType;
    IRBranchType branchType = IRBranchType::None;
    Ptr<IRVar> op1 =
        nullptr; ///< Stores: Name, Label, Param, Arg, lvalue of all OP
    Ptr<IRVar> op2 = nullptr; ///< Stores: RHS of assign, 1st OP of Expr
    Ptr<IRVar> op3 = nullptr; ///< Stores:
    IRFunction *target = nullptr;

    IRStmt *prev = nullptr;
    IRStmt *next = nullptr;
};

class IRFunction {
  public:
    IRFunction(IRIDType name_, Type *retTy_) : name{name_}, retTy{retTy_} {}

    IRIDType name;
    Type *retTy;
    IRVec<Ptr<IRVar>> varList;
    IRMap<IRIDType, Ptr<IRVar>> varMap;
    IRMap<IRIDType, Ptr<IRVar>> paramMap;
    IRVec<IRStmt> functionBody;
};

} // namespace splc

#endif // __SPLC_IR_IR_HH__
