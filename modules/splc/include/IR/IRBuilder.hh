#ifndef __SPLC_IR_IRBUILDER_HH__
#define __SPLC_IR_IRBUILDER_HH__ 1

#include "IR/IR.hh"
#include <algorithm>
#include <map>
#include <vector>

namespace splc {

class IRBuilder {
  public:
    IRBuilder(TypeContext &C) : context{C} {}

    void writeIRStmt(std::ostream &os, const IRStmt &stmt)
    {
        switch (stmt.irType) {
        case IRType::SetLabel: {
            os << "LABEL " << stmt.op1->name << " :\n";
            break;
        }
        case IRType::Assign: {
            os << stmt.op1->name << " := " << stmt.op2->name << "\n";
            break;
        }
        case IRType::Plus: {
            os << stmt.op1->name << " := " << stmt.op2->name << " + "
               << stmt.op3->name << "\n";
            break;
        }
        case IRType::Minus: {
            os << stmt.op1->name << " := " << stmt.op2->name << " - "
               << stmt.op3->name << "\n";
            break;
        }
        case IRType::Mul: {
            os << stmt.op1->name << " := " << stmt.op2->name << " * "
               << stmt.op3->name << "\n";
            break;
        }
        case IRType::Div: {
            os << stmt.op1->name << " := " << stmt.op2->name << " / "
               << stmt.op3->name << "\n";
            break;
        }
        case IRType::AddrOf: {
            os << stmt.op1->name << " := &" << stmt.op2->name << "\n";
            break;
        }
        case IRType::Deref: {
            os << stmt.op1->name << " := *" << stmt.op2->name << "\n";
            break;
        }
        case IRType::CopyToAddr: {
            os << "*" << stmt.op1->name << " := " << stmt.op2->name << "\n";
            break;
        }
        case IRType::Goto: {
            os << "GOTO " << stmt.op1->name << "\n";
            break;
        }
        case IRType::BranchIf: {
            os << "IF " << stmt.op1->name << " ";

            os << " " << stmt.op2->name << " GOTO " << stmt.op2->name << "\n";
            break;
        }
        case IRType::Return: {
            os << "RETURN " << stmt.op1->name << "\n";
            break;
        }
        case IRType::Alloc: {
            os << "DEC " << stmt.op1->name << " " << stmt.op2->name << "\n";
            break;
        }
        case IRType::PopCallArg: {
            os << "PARAM " << stmt.op1->name << "\n";
            break;
        }
        case IRType::PushCallArg: {
            os << "ARG " << stmt.op1->name << "\n";
            break;
        }
        case IRType::InvokeFunc: {
            os << stmt.op1->name << " := CALL " << stmt.target->name << "\n";
            break;
        }
        case IRType::Read: {
            os << "READ " << stmt.op1->name << "\n";
            break;
        }
        case IRType::Write: {
            os << "WRITE " << stmt.op1->name << "\n";
            break;
        }
        case IRType::FuncDecl: {
            splc_error();
            break;
        }
        }
    }

    void writeFunction(std::ostream &os, Ptr<IRFunction> func)
    {
        os << "FUNCTION " << func->name << " :\n";

        for (auto& param : func->paramMap) {
          os << "PARAM " << param.second->name << "\n";
        }

        for (auto& stmt : func->functionBody) {
          writeIRStmt(os, stmt);
        }
    }

    void writeAllFunctions(std::ostream &os)
    {
        // TODO: write all functions
    }

    void writeAllIRStmt(std::ostream &os, const IRStmt &begin)
    {
        // First, collect all functions
        // Second, write all functions,
        // Third, write all statements.

        // const IRStmt *stmt = &stmtRoot;
        // while (stmt != nullptr) {
        //     writeIRStmt(os, *stmt);
        //     stmt = stmt->next;
        // }
        // TODO: write all
    }

    void writeProgram(std::ostream &os)
    {
        // TODO:
    }

    void linkStmt(Ptr<IRStmt> prev, Ptr<IRStmt> next)
    {
        prev->next = next.get();
        next->prev = prev.get();
    }

    Ptr<IRVar> registerLabel(IRIDType id)
    {
        auto var = makeSharedPtr<IRVar>(id, IRVarType::Label);
        allLabels.insert(var);
        return var;
    }

    Ptr<IRVar> registerLabel(IRIDType id, Ptr<IRStmt> stmt)
    {
        auto var = registerLabel(id);
        bindLabelStmt(var, stmt);
        return var;
    }

    Ptr<IRVar> registerVariable(IRIDType id, Type *ty)
    {
        auto var = makeSharedPtr<IRVar>(id, IRVarType::Variable, ty);
        variableMap.insert({id, var});
        return var;
    }

    Ptr<IRFunction> registerFunction(IRIDType funcName, Type *retTy,
                                     const IRVec<Type *> &paramTys,
                                     const IRVec<IRIDType> ids)
    {
        splc_dbgassert(paramTys.size() == ids.size());
        // let us just assume all are integers.
        auto function = makeSharedPtr<IRFunction>(funcName, retTy);
        IRVec<IRPair<IRIDType, Ptr<IRVar>>> params;
        params.reserve(ids.size());
        std::transform(
            paramTys.begin(), paramTys.end(), ids.begin(),
            std::back_inserter(params), [](Type *pTy, IRIDType pID) {
                return IRPair<IRIDType, Ptr<IRVar>>{
                    pID, makeSharedPtr<IRVar>(pID, IRVarType::Variable, pTy)};
            });
        function->paramMap.insert(params.begin(), params.end());
        return function;
    }

    void insertStmt(Ptr<IRStmt> stmt) { allStmt.insert(stmt); }

    void bindLabelStmt(Ptr<IRVar> label, Ptr<IRStmt> stmt)
    {
        splc_dbgassert(label->irVarType == IRVarType::Label);
        labelStmtMap.insert({label->name, stmt});
    }

    Ptr<IRFunction> lookUpFunc(StrRef name)
    {
        auto it = funcMap.find(name);
        if (it == funcMap.end())
            return {nullptr};
        else
            return it->second;
    }

    Ptr<IRVar> lookUpVar(StrRef name)
    {
        auto it = variableMap.find(name);
        if (it == variableMap.end())
            return {nullptr};
        else
            return it->second;
    }

    Ptr<IRStmt> lookUpStmt(StrRef name)
    {
        auto it = labelStmtMap.find(name);
        return it->second;
    }

    void parseAST(PtrAST astRoot);

    TypeContext &context;
    IRSet<Ptr<IRVar>> allLabels;
    IRSet<Ptr<IRStmt>> allStmt;
    IRSet<IRFunction> allFunc;
    IRMap<IRIDType, Ptr<IRFunction>> funcMap;
    IRMap<IRIDType, Ptr<IRVar>> variableMap; ///< This is permanent
    IRMap<IRIDType, Ptr<IRStmt>> labelStmtMap;
};

} // namespace splc

#endif // __SPLC_IR_IRBUILDER_HH__
