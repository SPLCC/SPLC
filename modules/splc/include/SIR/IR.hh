#ifndef __SPLC_SIR_IR_HH__
#define __SPLC_SIR_IR_HH__ 1

#include <AST/DerivedAST.hh>
#include <Basic/Type.hh>
#include <Core/splc.hh>

#include <ranges>

namespace splc::SIR {

class IRVar;
class IRStmt;
class IRFunction;
class IRSB; // IR Super Block

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
    IRVar(IRIDType name_, IRVarType varType_ = IRVarType::Label,
          Type *type_ = nullptr, ASTValueType val_ = {}, bool isConst__ = false,
          bool isUsed__ = false)
        : name{name_}, irVarType{varType_}, valType{type_}, val{val_},
          isConst_{isConst__}, isUsed_{isUsed__}
    {
    }

    static PtrIRVar createLabel(IRIDType name);

    static PtrIRVar createFunction(IRIDType name, Type *type);

    static PtrIRVar createVariable(IRIDType name, Type *type);

    static PtrIRVar createConstant(Type *type, ASTValueType val);

    constexpr bool hasValue() const noexcept { return val.index() != 0; }

    template <IsValidASTValue T>
    auto getValue() noexcept
    {
        return std::get<T>(val);
    }

    template <IsValidASTValue T>
    auto getValue() const noexcept
    {
        return std::get<T>(val);
    }

    template <class Visitor>
    auto visitValue(Visitor &&vis) const noexcept
    {
        return std::visit(vis, val);
    }

    template <IsValidASTValue T>
    void emplaceValue(T &&val_) noexcept
    {
        val.emplace<T>(std::forward<T>(val_));
    }

    template <IsValidASTValue T>
    constexpr bool holdsValueType() const noexcept
    {
        return std::holds_alternative<T>(val);
    }

    void setConst() { isConst_ = true; }

    bool isConst() const { return isConst_; }

    void setUsed() { isUsed_ = true; }

    bool isUsed() const { return isUsed_; }

    std::string getName() const noexcept;

    // ASSUMPTION: NO SEMANTIC ERROR
    // ASSUMPTION: NO RVALUE ASSIGNMENT, THUS CONST PROPAGATION
    IRIDType name;
    IRVarType irVarType;
    Type *valType;
    ASTValueType val;
    bool isConst_;
    mutable bool isUsed_ = false;
};

// TODO: modify everything
class IRStmt {
  public:
    IRStmt() = delete;

    IRStmt(IRType irType_, PtrIRVar op1_, PtrIRVar op2_ = nullptr,
           PtrIRVar op3_ = nullptr,
           IRBranchType branchType_ = IRBranchType::None)
        : irType{irType_}, op1{op1_}, op2{op2_}, op3{op3_},
          branchType{branchType_}
    {
    }

    // IRStmt(IRType irType_, Ptr<IRFunction> func, PtrIRVar lhs_)
    //     : irType{irType_}, target{func.get()}, op1{lhs_}
    // {
    // }

    static PtrIRStmt createLabelStmt(PtrIRVar op);

    static PtrIRStmt createFuncDeclStmt(PtrIRVar op);

    static PtrIRStmt createAssignStmt(PtrIRVar lhs, PtrIRVar rhs);

    static PtrIRStmt createArithmeticStmt(IRType irType, PtrIRVar op1,
                                          PtrIRVar op2, PtrIRVar op3);

    static PtrIRStmt createAddrOfStmt(PtrIRVar op1, PtrIRVar op2);

    static PtrIRStmt createDerefStmt(PtrIRVar op1, PtrIRVar op2);

    static PtrIRStmt createCopyToAddrStmt(PtrIRVar op1, PtrIRVar op2);

    static PtrIRStmt createGotoStmt(PtrIRVar op);

    static PtrIRStmt createBranchIfStmt(IRBranchType branchType, PtrIRVar lhs,
                                        PtrIRVar rhs, PtrIRVar label);

    static PtrIRStmt createReturnStmt(PtrIRVar op);

    static PtrIRStmt createAllocStmt(PtrIRVar op1, PtrIRVar op2);

    static PtrIRStmt createPopCallArgStmt(PtrIRVar op);

    static PtrIRStmt createPushCallArgStmt(PtrIRVar op);

    static PtrIRStmt createInvokeFuncStmt(PtrIRVar lhs, PtrIRVar func);

    static PtrIRStmt createReadStmt(PtrIRVar op);

    static PtrIRStmt createWriteStmt(PtrIRVar op);

    IRType getIRType() const noexcept { return irType; }

    // clang-format off

    bool isSetLabel() const noexcept { return getIRType() == IRType::SetLabel; }

    bool isFuncDecl() const noexcept { return getIRType() == IRType::FuncDecl; }

    bool isAssign() const noexcept { return getIRType() == IRType::Assign; }

    bool isPlus() const noexcept { return getIRType() == IRType::Plus; }

    bool isMinus() const noexcept { return getIRType() == IRType::Minus; }

    bool isMul() const noexcept { return getIRType() == IRType::Mul; }

    bool isDiv() const noexcept { return getIRType() == IRType::Div; }

    bool isAddrOf() const noexcept { return getIRType() == IRType::AddrOf; }

    bool isDeref() const noexcept { return getIRType() == IRType::Deref; }

    bool isCopyToAddr() const noexcept { return getIRType() == IRType::CopyToAddr; }

    bool isGoto() const noexcept { return getIRType() == IRType::Goto; }

    bool isBranchIf() const noexcept { return getIRType() == IRType::BranchIf; }

    bool isReturn() const noexcept { return getIRType() == IRType::Return; }

    bool isAlloc() const noexcept { return getIRType() == IRType::Alloc; }

    bool isPopCallArg() const noexcept { return getIRType() == IRType::PopCallArg; }

    bool isPushCallArg() const noexcept { return getIRType() == IRType::PushCallArg; }

    bool isInvokeFunc() const noexcept { return getIRType() == IRType::InvokeFunc; }

    bool isRead() const noexcept { return getIRType() == IRType::Read; }

    bool isWrite() const noexcept { return getIRType() == IRType::Write; }

    // clang-format on

    friend std::ostream &operator<<(std::ostream &os,
                                    const IRStmt &stmt) noexcept;

    IRType irType;
    IRBranchType branchType = IRBranchType::None;
    PtrIRVar op1 = nullptr; ///< Stores: Name, Label,
                            ///< Param, Arg, lvalue of all OP
    PtrIRVar op2 = nullptr; ///< Stores: RHS of assign, 1st OP of Expr
    PtrIRVar op3 = nullptr; ///< Stores:
    // IRFunction *target = nullptr;

    IRStmt *prev = nullptr;
    IRStmt *next = nullptr;
};

class IRSB : public IRStmt {};

class IRFunction {
  public:
    IRFunction(IRIDType name_, Type *retTy_) : name{name_}, retTy{retTy_} {}

    static Ptr<IRFunction> create(IRIDType name_, Type *retTy_);

    friend std::ostream &operator<<(std::ostream &os,
                                    const IRFunction &func) noexcept;

    IRIDType name;
    Type *retTy;
    IRVec<PtrIRVar> varList;
    IRMap<IRIDType, PtrIRVar> varMap;
    IRVec<PtrIRVar> paramList;
    IRVec<PtrIRStmt> body;
};

class IRProgram {
  public:
    IRProgram(IRMap<IRIDType, Ptr<IRFunction>> funcMap_,
              IRMap<IRIDType, PtrIRVar> funcVarMap_) noexcept
        : funcMap{funcMap_}, funcVarMap{funcVarMap_}
    {
    }

    static Ptr<IRProgram> make(IRMap<IRIDType, Ptr<IRFunction>> funcMap_,
                               IRMap<IRIDType, PtrIRVar> funcVarMap_)
    {
        return makeSharedPtr<IRProgram>(funcMap_, funcVarMap_);
    }

    static void writeAllIRStmt(std::ostream &os, Ptr<IRProgram> program) noexcept
    {
        for (auto &func : program->funcMap) {
            os << *func.second << "\n";
        }
    }

    static void writeProgram(std::ostream &os, Ptr<IRProgram> program) noexcept
    {
        writeAllIRStmt(os, program);
    }

    IRMap<IRIDType, Ptr<IRFunction>> funcMap;
    IRMap<IRIDType, PtrIRVar> funcVarMap;
};

std::ostream &operator<<(std::ostream &os, const IRStmt &stmt) noexcept;

std::ostream &operator<<(std::ostream &os, const IRFunction &func) noexcept;
} // namespace splc::SIR

#endif // __SPLC_SIR_IR_HH__
