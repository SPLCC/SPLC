#ifndef __SPLC_IR_IR_HH__
#define __SPLC_IR_IR_HH__ 1

#include <AST/AST.hh>
#include <AST/Type.hh>
#include <Core/splc.hh>

namespace splc {

class IRVar;
class IRStmt;
class IRFunction;

using IRIDType = ASTIDType;
using StrRef = std::string_view;
using PtrIRVar = Ptr<IRVar>;
using PtrIRStmt = Ptr<IRStmt>;

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

enum class IRLogicalOpType {
    None,
    AND,
    OR,
    NOT,
    XOR,
};

class IRVar {
  public:
    static PtrIRVar createLabelVar(IRIDType name_);

    static PtrIRVar createFuncionVar(IRIDType name_, Type *type_);

    static PtrIRVar createVariableVar(IRIDType name_, Type *type_,
                                      bool isConst_ = false);

    static PtrIRVar createConstantVar(IRIDType name_, Type *type_);

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
        val.emplace<T>(std::forward<T>(val_));
    }

    template <IsValidASTValue T>
    constexpr bool holdsValueType() const noexcept
    {
        return std::holds_alternative<T>(val);
    }

    inline std::string getName()
    {
        if (irVarType == IRVarType::Variable && isConst) {
            if (holdsValueType<ASTSIntType>()) {
                return "#" + std::to_string(getValue<ASTSIntType>());
            }
            else if (holdsValueType<ASTUIntType>()) {

                return "#" + std::to_string(getValue<ASTUIntType>());
            }
            else if (holdsValueType<ASTIDType>()) {

                return getValue<ASTIDType>();
            }
        }
        return name;
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
    static PtrIRStmt createLabelStmt(PtrIRVar op1_);

    static PtrIRStmt createFuncDecl(PtrIRVar op1_);

    static PtrIRStmt createAssignStmt(PtrIRVar op1_, PtrIRVar op2_);

    static PtrIRStmt createArithmeticStmt(IRType irType, PtrIRVar op1_,
                                          Ptr<IRType> op2, Ptr<IRType> op3);

    static PtrIRStmt createAddrOfStmt(PtrIRVar op1_, PtrIRVar op2_);

    static PtrIRStmt createDerefStmt(PtrIRVar op1_, PtrIRVar op2_);

    static PtrIRStmt createCopyToAddrStmt(PtrIRVar op1_, PtrIRVar op2_);

    static PtrIRStmt createGotoStmt(PtrIRVar op1_);

    static PtrIRStmt createBranchIfStmt(IRBranchType branchType_, PtrIRVar lhs_,
                                        PtrIRVar rhs_, PtrIRVar label_);

    static PtrIRStmt createReturnStmt(PtrIRVar op1_);

    static PtrIRStmt createAllocStmt(PtrIRVar op1_, PtrIRVar op2_);

    static PtrIRStmt createPopCallArgStmt(PtrIRVar op1_);

    static PtrIRStmt createPushCallArgStmt(PtrIRVar op1_);

    static PtrIRStmt createInvokeFunc(Ptr<IRFunction> func,
                                      PtrIRVar lhs_ = nullptr);

    static PtrIRStmt createReadStmt(PtrIRVar op1_);

    static PtrIRStmt createWriteStmt(PtrIRVar op2_);

    friend std::ostream &operator<<(std::ostream &os, const IRStmt &stmt);

    IRType irType;
    IRBranchType branchType = IRBranchType::None;
    PtrIRVar op1 = nullptr; ///< Stores: Name, Label,
                            ///< Param, Arg, lvalue of all OP
    PtrIRVar op2 = nullptr; ///< Stores: RHS of assign, 1st OP of Expr
    PtrIRVar op3 = nullptr; ///< Stores:
    IRFunction *target = nullptr;

    IRStmt *prev = nullptr;
    IRStmt *next = nullptr;

  protected:
    IRStmt() = default;

    IRStmt(IRType irType_, PtrIRVar op1_, PtrIRVar op2_ = nullptr,
           PtrIRVar op3_ = nullptr,
           IRBranchType branchType_ = IRBranchType::None)
        : irType{irType_}, op1{op1_}, op2{op2_}, op3{op3_},
          branchType{branchType_}
    {
    }

    IRStmt(IRType irType_, Ptr<IRFunction> func, PtrIRVar lhs_)
        : irType{irType_}, target{func.get()}, op1{lhs_}
    {
    }
};

class IRFunction {
  public:
    static Ptr<IRFunction> create(IRIDType name_, Type *retTy_);

    IRFunction(IRIDType name_, Type *retTy_) : name{name_}, retTy{retTy_} {}

    IRIDType name;
    Type *retTy;
    IRVec<PtrIRVar> varList;
    IRMap<IRIDType, PtrIRVar> varMap;
    IRMap<IRIDType, PtrIRVar> paramMap;
    IRVec<PtrIRStmt> body;
};

inline std::ostream &operator<<(std::ostream &os, const IRStmt &stmt)
{
    switch (stmt.irType) {
    case IRType::SetLabel: {
        os << "LABEL " << stmt.op1->getName() << " :\n";
        break;
    }
    case IRType::Assign: {
        os << stmt.op1->getName() << " := " << stmt.op2->getName() << "\n";
        break;
    }
    case IRType::Plus: {
        os << stmt.op1->getName() << " := " << stmt.op2->getName() << " + "
           << stmt.op3->getName() << "\n";
        break;
    }
    case IRType::Minus: {
        os << stmt.op1->getName() << " := " << stmt.op2->getName() << " - "
           << stmt.op3->getName() << "\n";
        break;
    }
    case IRType::Mul: {
        os << stmt.op1->getName() << " := " << stmt.op2->getName() << " * "
           << stmt.op3->getName() << "\n";
        break;
    }
    case IRType::Div: {
        os << stmt.op1->getName() << " := " << stmt.op2->getName() << " / "
           << stmt.op3->getName() << "\n";
        break;
    }
    case IRType::AddrOf: {
        os << stmt.op1->getName() << " := &" << stmt.op2->getName() << "\n";
        break;
    }
    case IRType::Deref: {
        os << stmt.op1->getName() << " := *" << stmt.op2->getName() << "\n";
        break;
    }
    case IRType::CopyToAddr: {
        os << "*" << stmt.op1->getName() << " := " << stmt.op2->getName()
           << "\n";
        break;
    }
    case IRType::Goto: {
        os << "GOTO " << stmt.op1->getName() << "\n";
        break;
    }
    case IRType::BranchIf: {
        os << "IF " << stmt.op1->getName() << " ";
        switch (stmt.branchType) {
        case IRBranchType::None: {
            splc_error();
            break;
        }
        case IRBranchType::LT: {
            os << "<";
            break;
        }
        case IRBranchType::LE: {
            os << "<=";
            break;
        }
        case IRBranchType::GT: {
            os << ">";
            break;
        }
        case IRBranchType::GE: {
            os << ">=";
            break;
        }
        case IRBranchType::EQ: {
            os << "==";
            break;
        }
        case IRBranchType::NE: {
            os << "!=";
            break;
        }
        }
        os << " " << stmt.op2->getName() << " GOTO " << stmt.op3->getName()
           << "\n";
        break;
    }
    case IRType::Return: {
        os << "RETURN " << stmt.op1->getName() << "\n";
        break;
    }
    case IRType::Alloc: {
        os << "DEC " << stmt.op1->getName() << " " << stmt.op2->getName()
           << "\n";
        break;
    }
    case IRType::PopCallArg: {
        os << "PARAM " << stmt.op1->getName() << "\n";
        break;
    }
    case IRType::PushCallArg: {
        os << "ARG " << stmt.op1->getName() << "\n";
        break;
    }
    case IRType::InvokeFunc: {
        os << stmt.op1->getName() << " := CALL " << stmt.op2->getName() << "\n";
        break;
    }
    case IRType::Read: {
        os << "READ " << stmt.op1->getName() << "\n";
        break;
    }
    case IRType::Write: {
        os << "WRITE " << stmt.op1->getName() << "\n";
        break;
    }
    case IRType::FuncDecl: {
        splc_error();
        break;
    }
    default: {
        splc_error();
        break;
    }
    }
    return os;
}

inline std::ostream &operator<<(std::ostream &os, const IRFunction &func) 
{
    os << "FUNCTION " << func.name << " :\n";
    // reverse pop
    for (auto it = func.paramMap.rbegin(); it != func.paramMap.rend(); ++it) {
        os << "PARAM " << it->first << "\n";
    }

    for (auto &var : func.varList) {

    }
}

} // namespace splc

#endif // __SPLC_IR_IR_HH__
