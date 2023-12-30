#ifndef __SPLC_IR_IRBUILDER_HH__
#define __SPLC_IR_IRBUILDER_HH__ 1

#include "AST/ASTContext.hh"
#include "IR/IR.hh"
#include <algorithm>
#include <map>

namespace splc {

class IRBuilder;
class IRBuilderHelper;

class IRBuilder {
  public:
    IRBuilder(TypeContext &C) : tyCtxt(C) {}

    void writeAllIRStmt(std::ostream &os)
    {
        for (const auto &func : funcMap) {
            os << *func.second << "\n";
        }
    }

    void writeProgram(std::ostream &os) { writeAllIRStmt(os); }

    // ------------------------ register ------------------------
    // register function
    void registerFunction(PtrAST funcRoot);

    // register stmt
    void recRegisterStmts(IRVec<PtrIRStmt> &stmtList, PtrAST stmtRoot);
    void recRegisterIterStmt(IRVec<PtrIRStmt> &stmtList, PtrAST stmtRoot);
    void recRegisterSelStmt(IRVec<PtrIRStmt> &stmtList, PtrAST stmtRoot);
    void recRegisterJumpStmt(IRVec<PtrIRStmt> &stmtList, PtrAST stmtRoot);

    // register expr
    PtrIRVar recRegisterExprs(IRVec<PtrIRStmt> &stmtList, PtrAST exprRoot);
    PtrIRVar recRegisterCallExpr(IRVec<PtrIRStmt> &stmtList, PtrAST exprRoot);
    void recRegisterCondExpr(IRVec<PtrIRStmt> &stmtList, PtrAST exprRoot,
                             PtrIRVar lbt, PtrIRVar lbf);

    // register declaration
    void recRegisterDeclVar(IRVec<PtrIRStmt> &stmtList, PtrAST declRoot);
    // ----------------------------------------------------------

    void parseAST(PtrAST parseRoot);

    PtrIRVar getTmpLabel();
    PtrIRVar getTmpVar();

    size_t allocCnt = 0;
    TypeContext &tyCtxt;
    Ptr<ASTContext> rootCtxt;

    IRMap<IRIDType, Ptr<IRFunction>> funcMap;
    IRMap<IRIDType, PtrIRVar> funcVarMap;

    IRVec<PtrIRVar> varList;
    IRMap<IRIDType, PtrIRVar> varMap;
};

class IRBuilderHelper {
  public:
    static IRVec<IRIDType> recfindFuncParam(PtrAST funcRoot)
    {
        IRVec<IRIDType> vec;
        _recfindFuncParam(vec, funcRoot);
        return vec;
    }

  protected:
    static void _recfindFuncParam(IRVec<IRIDType> &vec, PtrAST funcRoot)
    {
        ASTSymType type = funcRoot->getSymType();
        if (type == ASTSymType::FuncDef && funcRoot->getChildrenNum() == 3) {
            _recfindFuncParam(vec, funcRoot->getChildren()[1]);
        }
        else if (type == ASTSymType::DirFuncDecltr) {
            _recfindFuncParam(vec, funcRoot->getChildren()[1]);
        }
        else if (isASTSymbolTypeOneOf(type, ASTSymType::FuncDecltr,
                                      ASTSymType::ParamTypeList)) {
            _recfindFuncParam(vec, funcRoot->getChildren()[0]);
        }
        else if (type == ASTSymType::ParamList) {
            for (auto &param : funcRoot->getChildren()) {
                // ParamDecltr
                vec.push_back(param->getChildren()[1]
                                  ->getChildren()[0]
                                  ->getChildren()[0]
                                  ->getConstVal<IRIDType>());
            }
        }
    }
};

inline void linkStmt(PtrIRStmt prev, PtrIRStmt next)
{
    prev->next = next.get();
    next->prev = prev.get();
}

} // namespace splc

#endif // __SPLC_IR_IRBUILDER_HH__
