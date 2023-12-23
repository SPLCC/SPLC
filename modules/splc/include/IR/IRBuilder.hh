#ifndef __SPLC_IR_IRBUILDER_HH__
#define __SPLC_IR_IRBUILDER_HH__ 1

#include "AST/ASTContext.hh"
#include "IR/IR.hh"
#include <algorithm>
#include <map>

namespace splc {

// TODO: The entire IRBuilder is essentially trash. Try refactor entirely.
class IRBuilder {
  public:
    IRBuilder(TypeContext &C) : tyCtxt{C} {}

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

        for (auto &param : func->paramMap) {
            os << "PARAM " << param.second->name << "\n";
        }

        for (auto &stmt : func->functionBody) {
            writeIRStmt(os, *stmt);
        }
    }

    void writeAllIRStmt(std::ostream &os)
    {
        for (auto &func : funcMap) {
            writeFunction(os, func.second);
        }
    }

    void writeProgram(std::ostream &os) { writeAllIRStmt(os); }

    Ptr<IRFunction> registerFunction(IRIDType funcName, Type *retTy,
                                     const IRVec<Type *> &paramTys,
                                     const IRVec<IRIDType> paramIDs)
    {
        splc_dbgassert(paramTys.size() == paramIDs.size());
        // let us just assume all are integers.
        auto function = makeSharedPtr<IRFunction>(funcName, retTy);
        IRVec<IRPair<IRIDType, Ptr<IRVar>>> params;
        params.reserve(paramIDs.size());
        std::transform(
            paramTys.begin(), paramTys.end(), paramIDs.begin(),
            std::back_inserter(params), [](Type *pTy, IRIDType pID) {
                return IRPair<IRIDType, Ptr<IRVar>>{
                    pID, makeSharedPtr<IRVar>(pID, IRVarType::Variable, pTy)};
            });
        function->paramMap.insert(params.begin(), params.end());
        return function;
    }

    void parseAST(PtrAST astRoot);

    size_t allocCnt = 0;
    TypeContext &tyCtxt;
    Ptr<ASTContext> rootCtxt; ///< TODO
    IRMap<IRIDType, Ptr<IRFunction>> funcMap;

    // private:

    Ptr<IRVar> getTmpVar();

    void parseExpr(PtrAST exprRoot);

    void parseDecl(PtrAST declRoot);

    void recRegisterWhileLoop(IRVec<Ptr<IRVar>> &varList,
                              IRMap<IRIDType, Ptr<IRVar>> &varMap,
                              IRVec<Ptr<IRStmt>> &stmtList, PtrAST loopRoot);

    void recRegisterIfStmt(IRVec<Ptr<IRVar>> &varList,
                           IRMap<IRIDType, Ptr<IRVar>> &varMap,
                           IRVec<Ptr<IRStmt>> &stmtList, PtrAST loopRoot);

    void recRegisterInitzrExpr(IRMap<IRIDType, Ptr<IRVar>> &varMap,
                               IRVec<Ptr<IRStmt>> &stmtList, Ptr<IRVar> var,
                               PtrAST dirDecltr);

    void recRegisterInitDecltr(IRMap<IRIDType, Ptr<IRVar>> &varMap,
                               IRVec<Ptr<IRStmt>> &stmtList, PtrAST dirDecltr);

    void recRegisterDecl(IRMap<IRIDType, Ptr<IRVar>> &varMap,
                         IRVec<Ptr<IRStmt>> &stmtList, PtrAST declRoot);

    void recfindFuncDecls(IRVec<Ptr<IRVar>> &varList,
                          IRMap<IRIDType, Ptr<IRVar>> &varMap, PtrAST funcRoot,
                          IRIDType funcID);

    Ptr<IRVar> recRegisterStmts(IRVec<Ptr<IRVar>> &varList,
                                IRMap<IRIDType, Ptr<IRVar>> &varMap,
                                IRVec<Ptr<IRStmt>> &stmtList, PtrAST stmtRoot);

    void parseFunction(PtrAST funcRoot);

    void recursiveParseAST(PtrAST parseRoot);
};

inline void linkStmt(Ptr<IRStmt> prev, Ptr<IRStmt> next)
{
    prev->next = next.get();
    next->prev = prev.get();
}

} // namespace splc

#endif // __SPLC_IR_IRBUILDER_HH__
