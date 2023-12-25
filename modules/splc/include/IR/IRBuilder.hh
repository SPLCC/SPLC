#ifndef __SPLC_IR_IRBUILDER_HH__
#define __SPLC_IR_IRBUILDER_HH__ 1

#include "AST/ASTContext.hh"
#include "IR/IR.hh"
#include <algorithm>
#include <map>

namespace splc {

class IRBuilder;
class IRBuilderHelper;

// TODO: The entire IRBuilder is essentially trash. Try refactor entirely.
class IRBuilder {
  public:
    IRBuilder(TypeContext &C) : tyCtxt(C) {}

    void writeFunction(std::ostream &os, Ptr<IRFunction> func);

    void writeAllIRStmt(std::ostream &os);

    void writeProgram(std::ostream &os) { writeAllIRStmt(os); }

    void registerFunction(PtrAST funcRoot);

    // register stmt
    void recRegisterStmts(IRVec<PtrIRStmt> stmtList, PtrAST stmtRoot);
    void recRegisterIterStmt(IRVec<PtrIRStmt> stmtList, PtrAST stmtRoot);
    void recRegisterSelStmt(IRVec<PtrIRStmt> stmtList, PtrAST stmtRoot);
    void recRegisterJumpStmt(IRVec<PtrIRStmt> stmtList, PtrAST stmtRoot);

    // register expr
    void recRegisterExprs(IRVec<PtrIRStmt> stmtList, PtrAST exprRoot);
    // TODO: May can combine
    PtrIRVar recRegisterBasicExpr(IRVec<PtrIRStmt> stmtList, PtrAST exprRoot);
    IRPair<PtrIRVar, PtrIRVar> recRegisterCondExpr(IRVec<PtrIRStmt> stmtList,
                                                   PtrAST exprRoot);

    void recRegisterInitDecltr(IRVec<PtrIRStmt> stmtList, PtrAST dirDecltr);

    void recRegisterDeclVal(IRVec<PtrIRStmt> stmtList, PtrAST declRoot);

    void recursiveParseAST(PtrAST parseRoot);

    PtrIRVar getTmpLabel();
    PtrIRVar getTmpVar();

    size_t allocCnt = 0;
    TypeContext &tyCtxt;
    Ptr<ASTContext> rootCtxt;
    IRMap<IRIDType, Ptr<IRFunction>> funcMap;

    IRVec<PtrIRVar> varList;
    IRMap<IRIDType, PtrIRVar> varMap;
};

class IRBuilderHelper {
  public:
    static IRIDType recfindFuncID(PtrAST funcRoot);

    static IRVec<IRIDType> recfindFuncParam(PtrAST funcRoot)
    {
        IRVec<IRIDType> vec;
        _recfindFuncParam(vec, funcRoot);
        return vec;
    }

    static IRBranchType reverseBranchType(ASTSymbolType type)
    {
        IRBranchType branchType;
        switch (type) {
        case ASTSymbolType::OpLT: {
            branchType = IRBranchType::GE;
            break;
        }
        case ASTSymbolType::OpLE: {
            branchType = IRBranchType::GT;
            break;
        }
        case ASTSymbolType::OpGT: {
            branchType = IRBranchType::LE;
            break;
        }
        case ASTSymbolType::OpGE: {
            branchType = IRBranchType::LT;
            break;
        }
        case ASTSymbolType::OpEQ: {
            branchType = IRBranchType::NE;
            break;
        }
        case ASTSymbolType::OpNE: {
            branchType = IRBranchType::EQ;
            break;
        }
        default:
            splc_error();
        }
        return branchType;
    }

  protected:
    static void _recfindFuncParam(IRVec<IRIDType> &vec, PtrAST funcRoot);
};

inline void linkStmt(PtrIRStmt prev, PtrIRStmt next)
{
    prev->next = next.get();
    next->prev = prev.get();
}

} // namespace splc

#endif // __SPLC_IR_IRBUILDER_HH__
