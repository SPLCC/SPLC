#ifndef __SPLC_SIR_IRBUILDER_HH__
#define __SPLC_SIR_IRBUILDER_HH__ 1

#include "AST/ASTContext.hh"
#include "SIR/IR.hh"
#include <algorithm>
#include <map>

namespace splc::SIR {

class IRBuilder;
class IRBuilderHelper;

class IRBuilder {
  public:
    IRBuilder(SPLCContext &C) noexcept : tyCtxt(C) {}

    PtrIRVar getTmpLabel();
    PtrIRVar getTmpVar();

    // ------------------------ register ------------------------

    // register declaration

    void recRegisterDeclVar(IRVec<PtrIRStmt> &stmtList, PtrAST declRoot);

    // register expr

    PtrIRVar recRegisterExprs(IRVec<PtrIRStmt> &stmtList, PtrAST exprRoot);
    PtrIRVar recRegisterCallExpr(IRVec<PtrIRStmt> &stmtList, PtrAST exprRoot);
    void recRegisterCondExpr(IRVec<PtrIRStmt> &stmtList, PtrAST exprRoot,
                             PtrIRVar lbt, PtrIRVar lbf);

    // register stmt

    void recRegisterIterStmt(IRVec<PtrIRStmt> &stmtList, PtrAST stmtRoot);
    void recRegisterSelStmt(IRVec<PtrIRStmt> &stmtList, PtrAST stmtRoot);
    void recRegisterJumpStmt(IRVec<PtrIRStmt> &stmtList, PtrAST stmtRoot);

    void recRegisterStmts(IRVec<PtrIRStmt> &stmtList, PtrAST stmtRoot);

    // register function

    void registerFunction(PtrAST funcRoot);

    // ----------------------------------------------------------

    void parseAST(PtrAST parseRoot);

    void writeAllIRStmt(std::ostream &os) const noexcept
    {
        for (auto &func : funcMap) {
            os << *func.second;
        }
    }

    void writeProgram(std::ostream &os) const noexcept { writeAllIRStmt(os); }

    size_t allocCnt = 0;
    SPLCContext &tyCtxt;
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

} // namespace splc::SIR

#endif // __SPLC_SIR_IRBUILDER_HH__
