#include "SIR/IR.hh"

namespace splc::SIR {

//===----------------------------------------------------------------------===//
//                             IRVar Implementation
//===----------------------------------------------------------------------===//
// Create IRVar

PtrIRVar IRVar::createLabel(IRIDType name)
{
    return makeSharedPtr<IRVar>(name, IRVarType::Label);
}

PtrIRVar IRVar::createFunction(IRIDType name, Type *type)
{
    return makeSharedPtr<IRVar>(name, IRVarType::Function, type);
}

PtrIRVar IRVar::createVariable(IRIDType name, Type *type)
{
    return makeSharedPtr<IRVar>(name, IRVarType::Variable, type);
}

PtrIRVar IRVar::createConstant(Type *type, ASTValueType val)
{
    return makeSharedPtr<IRVar>("", IRVarType::Constant, type, val, true);
}

std::string IRVar::getName() const noexcept
{
    if ((irVarType == IRVarType::Variable && isConst_) ||
        irVarType == IRVarType::Constant) {
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

//===----------------------------------------------------------------------===//
//                             IRStmt Implementation
//===----------------------------------------------------------------------===//
// Create IRStmt

PtrIRStmt IRStmt::createLabelStmt(PtrIRVar op)
{
    op->setUsed();
    return makeSharedPtr<IRStmt>(IRType::SetLabel, op);
}

PtrIRStmt IRStmt::createFuncDeclStmt(PtrIRVar op)
{
    op->setUsed();
    return makeSharedPtr<IRStmt>(IRType::FuncDecl, op);
}

PtrIRStmt IRStmt::createAssignStmt(PtrIRVar lhs, PtrIRVar rhs)
{
    rhs->setUsed();
    return makeSharedPtr<IRStmt>(IRType::Assign, lhs, rhs);
}

PtrIRStmt IRStmt::createArithmeticStmt(IRType irType, PtrIRVar op1,
                                       PtrIRVar op2, PtrIRVar op3)
{
    op2->setUsed();
    op3->setUsed();
    return makeSharedPtr<IRStmt>(irType, op1, op2, op3);
}

PtrIRStmt IRStmt::createAddrOfStmt(PtrIRVar op1, PtrIRVar op2)
{
    op2->setUsed();
    return makeSharedPtr<IRStmt>(IRType::AddrOf, op1, op2);
}

PtrIRStmt IRStmt::createDerefStmt(PtrIRVar op1, PtrIRVar op2)
{
    op2->setUsed();
    return makeSharedPtr<IRStmt>(IRType::Deref, op1, op2);
}

PtrIRStmt IRStmt::createCopyToAddrStmt(PtrIRVar op1, PtrIRVar op2)
{
    op2->setUsed();
    return makeSharedPtr<IRStmt>(IRType::CopyToAddr, op1, op2);
}

PtrIRStmt IRStmt::createGotoStmt(PtrIRVar op)
{
    op->setUsed();
    return makeSharedPtr<IRStmt>(IRType::Goto, op);
}

PtrIRStmt IRStmt::createBranchIfStmt(IRBranchType branchType, PtrIRVar lhs,
                                     PtrIRVar rhs, PtrIRVar label)
{
    lhs->setUsed();
    rhs->setUsed();
    return makeSharedPtr<IRStmt>(IRType::BranchIf, lhs, rhs, label, branchType);
}

PtrIRStmt IRStmt::createReturnStmt(PtrIRVar op)
{
    op->setUsed();
    return makeSharedPtr<IRStmt>(IRType::Return, op);
}

PtrIRStmt IRStmt::createAllocStmt(PtrIRVar op1, PtrIRVar op2)
{
    op1->setUsed();
    op2->setUsed();
    return makeSharedPtr<IRStmt>(IRType::Alloc, op1, op2);
}

PtrIRStmt IRStmt::createPopCallArgStmt(PtrIRVar op)
{
    op->setUsed();
    return makeSharedPtr<IRStmt>(IRType::PopCallArg, op);
}

PtrIRStmt IRStmt::createPushCallArgStmt(PtrIRVar op)
{
    op->setUsed();
    return makeSharedPtr<IRStmt>(IRType::PushCallArg, op);
}

PtrIRStmt IRStmt::createInvokeFuncStmt(PtrIRVar lhs, PtrIRVar func)
{
    func->setUsed();
    return makeSharedPtr<IRStmt>(IRType::InvokeFunc, lhs, func);
}

PtrIRStmt IRStmt::createReadStmt(PtrIRVar op)
{
    op->setUsed();
    return makeSharedPtr<IRStmt>(IRType::Read, op);
}

PtrIRStmt IRStmt::createWriteStmt(PtrIRVar op)
{
    op->setUsed();
    return makeSharedPtr<IRStmt>(IRType::Write, op);
}

//===----------------------------------------------------------------------===//
//                         IRFunction Implementation
//===----------------------------------------------------------------------===//
// Create IR Function

Ptr<IRFunction> IRFunction::create(IRIDType name_, Type *retTy_)
{
    return makeSharedPtr<IRFunction>(name_, retTy_);
}

std::ostream &operator<<(std::ostream &os, const IRStmt &stmt) noexcept
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

std::ostream &operator<<(std::ostream &os, const IRFunction &func) noexcept
{
    os << "FUNCTION " << func.name << " :\n";
    for (auto stmt : func.body) {
        os << *stmt;
    }
    return os;
}

} // namespace splc::SIR