#include "IR/IR.hh"

using namespace splc;

// ------------------------ Create IRVar ------------------------------
PtrIRVar IRVar::createLabelVar(IRIDType name)
{
    return makeSharedPtr<IRVar>(name, IRVarType::Label);
}

PtrIRVar IRVar::createFuncionVar(IRIDType name, Type *type)
{
    return makeSharedPtr<IRVar>(name, IRVarType::Function, type);
}

PtrIRVar IRVar::createVariableVar(IRIDType name, Type *type)
{
    return makeSharedPtr<IRVar>(name, IRVarType::Variable, type);
}

PtrIRVar IRVar::createConstantVar(Type *type, ASTValueType val)
{
    return makeSharedPtr<IRVar>("", IRVarType::Constant, type, val, true);
}

// ------------------------ Create IRStmt ------------------------------
PtrIRStmt IRStmt::createLabelStmt(PtrIRVar op)
{
    return makeSharedPtr<IRStmt>(IRType::SetLabel, op);
}
PtrIRStmt IRStmt::createFuncDeclStmt(PtrIRVar op)
{
    return makeSharedPtr<IRStmt>(IRType::FuncDecl, op);
}
PtrIRStmt IRStmt::createAssignStmt(PtrIRVar lhs, PtrIRVar rhs)
{
    return makeSharedPtr<IRStmt>(IRType::Assign, lhs, rhs);
}
PtrIRStmt IRStmt::createArithmeticStmt(IRType irType, PtrIRVar op1,
                                       PtrIRVar op2, PtrIRVar op3)
{
    return makeSharedPtr<IRStmt>(irType, op1, op2, op3);
}
PtrIRStmt IRStmt::createAddrOfStmt(PtrIRVar op1, PtrIRVar op2)
{
    return makeSharedPtr<IRStmt>(IRType::AddrOf, op1, op2);
}
PtrIRStmt IRStmt::createDerefStmt(PtrIRVar op1, PtrIRVar op2)
{
    return makeSharedPtr<IRStmt>(IRType::Deref, op1, op2);
}
PtrIRStmt IRStmt::createCopyToAddrStmt(PtrIRVar op1, PtrIRVar op2)
{
    return makeSharedPtr<IRStmt>(IRType::CopyToAddr, op1, op2);
}
PtrIRStmt IRStmt::createGotoStmt(PtrIRVar op)
{
    return makeSharedPtr<IRStmt>(IRType::Goto, op);
}
PtrIRStmt IRStmt::createBranchIfStmt(IRBranchType branchType, PtrIRVar lhs,
                                     PtrIRVar rhs, PtrIRVar label)
{
    return makeSharedPtr<IRStmt>(IRType::BranchIf, lhs, rhs, label, branchType);
}
PtrIRStmt IRStmt::createReturnStmt(PtrIRVar op)
{
    return makeSharedPtr<IRStmt>(IRType::Return, op);
}
PtrIRStmt IRStmt::createAllocStmt(PtrIRVar op1, PtrIRVar op2)
{
    return makeSharedPtr<IRStmt>(IRType::Alloc, op1, op2);
}
PtrIRStmt IRStmt::createPopCallArgStmt(PtrIRVar op)
{
    return makeSharedPtr<IRStmt>(IRType::PopCallArg, op);
}
PtrIRStmt IRStmt::createPushCallArgStmt(PtrIRVar op)
{
    return makeSharedPtr<IRStmt>(IRType::PushCallArg, op);
}
PtrIRStmt IRStmt::createInvokeFuncStmt(PtrIRVar lhs, PtrIRVar func)
{
    return makeSharedPtr<IRStmt>(IRType::InvokeFunc, lhs, func);
}
PtrIRStmt IRStmt::createReadStmt(PtrIRVar op)
{
    return makeSharedPtr<IRStmt>(IRType::Read, op);
}
PtrIRStmt IRStmt::createWriteStmt(PtrIRVar op)
{
    return makeSharedPtr<IRStmt>(IRType::Write, op);
}

// ------------------------ Create IRFunction ---------------------------
Ptr<IRFunction> IRFunction::create(IRIDType name_, Type *retTy_) {
    return makeSharedPtr<IRFunction>(name_, retTy_);
}