#include "IR/IRBuilder.hh"
#include "AST/AST.hh"
#include "IR/IR.hh"

using namespace splc;

Ptr<IRVar> registerVariable(IRIDType id, Type *ty)
{
    return makeSharedPtr<IRVar>(id, IRVarType::Variable, ty);
}

Ptr<IRVar> IRBuilder::IRBuilder::getTmpVar()
{
    return registerVariable("tmp_" + std::to_string(allocCnt++),
                            &tyCtxt.SInt32Ty);
}

IRIDType getNearestIDBelow(PtrAST declRoot)
{
    for (auto &child : declRoot->getChildren()) {
        if (isASTSymbolTypeOneOfThem(
                child->getSymbolType(), ASTSymbolType::InitDecltr,
                ASTSymbolType::Decltr, ASTSymbolType::DirDecltr)) {
            if (auto s = getNearestIDBelow(child); !s.empty())
                return s;
        }
        else if (child->getSymbolType() == ASTSymbolType::ID) {
            return child->getValue<IRIDType>();
        }
    }
    return {};
}

IRIDType recfindFuncID(PtrAST funcRoot)
{
    for (auto &child : funcRoot->getChildren()) {
        if (isASTSymbolTypeOneOfThem(child->getSymbolType(),
                                     ASTSymbolType::FuncDecltr,
                                     ASTSymbolType::DirFuncDecltr)) {
            return recfindFuncID(child);
        }
        else if (child->getSymbolType() == ASTSymbolType::ID) {
            return child->getValue<IRIDType>();
        }
    }
    splc_error() << "invalid function decl";
    splc_unreachable();
}

void recfindFuncParam(IRVec<IRIDType> &vec, PtrAST funcRoot)
{
    for (auto &child : funcRoot->getChildren()) {
        if (isASTSymbolTypeOneOfThem(
                child->getSymbolType(), ASTSymbolType::FuncDecltr,
                ASTSymbolType::DirFuncDecltr, ASTSymbolType::ParamTypeList,
                ASTSymbolType::ParamList, ASTSymbolType::ParamDecltr,
                ASTSymbolType::Decltr, ASTSymbolType::DirDecltr)) {
            recfindFuncParam(vec, child);
        }
        else if (child->getSymbolType() == ASTSymbolType::ID) {
            vec.push_back(child->getValue<IRIDType>());
        }
    }
}

// Now try to parse all declarations
void IRBuilder::recfindFuncDecls(IRVec<Ptr<IRVar>> &varList,
                                 IRMap<IRIDType, Ptr<IRVar>> &varMap,
                                 PtrAST funcRoot, IRIDType funcID)
{
    auto &vec = rootCtxt->getSymbolList();
    bool tryFindDecl = false;
    for (auto &ent : vec) {
        if (tryFindDecl && ent.second.symEntTy == SymEntryType::Variable) {
            const auto &id = ent.first;
            const auto val =
                static_cast<int>(ent.second.getValue<ASTUIntType>());
            Ptr<IRVar> var =
                makeSharedPtr<IRVar>(id, IRVarType::Variable, &tyCtxt.SInt32Ty,
                                     val, ent.second.hasValue());
            varList.push_back(var);
            varMap.insert({id, var});
        }
        else if (ent.second.symEntTy == SymEntryType::Function) {
            if (ent.first == funcID) {
                tryFindDecl = true;
            }
            else if (tryFindDecl) {
                break;
            }
        }
    }
}

void IRBuilder::recRegisterWhileLoop(IRVec<Ptr<IRVar>> &varList,
                                     IRMap<IRIDType, Ptr<IRVar>> &varMap,
                                     IRVec<Ptr<IRStmt>> &stmtList,
                                     PtrAST loopRoot)
{

}

void IRBuilder::recRegisterIfStmt(IRVec<Ptr<IRVar>> &varList,
                                  IRMap<IRIDType, Ptr<IRVar>> &varMap,
                                  IRVec<Ptr<IRStmt>> &stmtList, PtrAST loopRoot)
{

}

void IRBuilder::recRegisterInitzrExpr(IRMap<IRIDType, Ptr<IRVar>> &varMap,
                                      IRVec<Ptr<IRStmt>> &stmtList,
                                      Ptr<IRVar> var, PtrAST exprRoot)
{
    splc_dbgassert(exprRoot->getSymbolType() == ASTSymbolType::Expr);
    if (auto &child = exprRoot->getChildren()[0];
        child->getSymbolType() == ASTSymbolType::ID) {
        auto it2 = varMap.find(child->getValue<IRIDType>());
        splc_dbgassert(it2 != varMap.end());
        Ptr<IRStmt> stmt =
            makeSharedPtr<IRStmt>(IRType::Assign, var, it2->second);
        stmtList.push_back(stmt);
        return;
    }
    else if (auto &child = exprRoot->getChildren()[0];
             child->getSymbolType() == ASTSymbolType::Constant) {
        // do nothing
        return;
    }
    splc_error();
    splc_unreachable();
}

void IRBuilder::recRegisterInitDecltr(IRMap<IRIDType, Ptr<IRVar>> &varMap,
                                      IRVec<Ptr<IRStmt>> &stmtList, PtrAST root)
{
    splc_dbgassert(root->getSymbolType() == ASTSymbolType::InitDecltr);
    // get ID first
    auto id = getNearestIDBelow(root);
    auto it = varMap.find(id);
    splc_dbgassert(it != varMap.end());

    if (root->getChildrenNum() == 3 &&
        root->getChildren()[2]->getChildrenNum() > 0) {
        // There is an initializer
        auto &initExpr = root->getChildren()[2]->getChildren()[0];
        recRegisterInitzrExpr(varMap, stmtList, it->second, initExpr);
    }
}

void IRBuilder::recRegisterDecl(IRMap<IRIDType, Ptr<IRVar>> &varMap,
                                IRVec<Ptr<IRStmt>> &stmtList, PtrAST declRoot)
{
    for (auto &child : declRoot->getChildren()) {
        if (isASTSymbolTypeOneOfThem(child->getSymbolType(),
                                     ASTSymbolType::DirDecl)) {
            recRegisterDecl(varMap, stmtList, child);
        }
        else if (isASTSymbolTypeOneOfThem(child->getSymbolType(),
                                          ASTSymbolType::InitDecltrList)) {
            for (auto &initDecltrs : child->getChildren()) {
                recRegisterInitDecltr(varMap, stmtList, initDecltrs);
            }
        }
    }
}

// Now try to parse all expressions
void IRBuilder::recRegisterExprs(IRVec<Ptr<IRVar>> &varList,
                                 IRMap<IRIDType, Ptr<IRVar>> &varMap,
                                 IRVec<Ptr<IRStmt>> &stmtList, PtrAST stmtRoot)
{
    if (stmtRoot->getSymbolType() == ASTSymbolType::GeneralStmtList) {
        for (auto &child : stmtRoot->getChildren()) {
            recRegisterExprs(varList, varMap, stmtList, child);
        }
    }
    else if (stmtRoot->getSymbolType() == ASTSymbolType::Decl) {
        for (auto &child : stmtRoot->getChildren()) {
            recRegisterDecl(varMap, stmtList, child);
            // ASSIGN INITIAL VALUES, IF NONCONSTEXPR
        }
    }
    else if (stmtRoot->getSymbolType() == ASTSymbolType::Expr) {
        // register expression
        if (stmtRoot->getChildrenNum() == 2) {
            // assignment
            // TODO
        }
        else if (stmtRoot->getChildrenNum() == 3) {
            // either conditional or assignment
            // TODO
        }
        splc_error();
        splc_unreachable();
    }
}

void IRBuilder::parseFunction(PtrAST funcRoot)
{
    IRIDType funcID = recfindFuncID(funcRoot);
    IRVec<IRIDType> paramIDs;
    recfindFuncParam(paramIDs, funcRoot);
    IRVec<Type *> paramTys{paramIDs.size(), &tyCtxt.SInt32Ty};

    // Now try to parse all declarations
    IRVec<Ptr<IRVar>> varList;
    IRMap<IRIDType, Ptr<IRVar>> varMap;
    IRVec<Ptr<IRStmt>> stmtList; // just insert them linearly

    recfindFuncDecls(varList, varMap, funcRoot, funcID);
    // just find the function root first
    auto &body = funcRoot->getChildren()[2];
    splc_dbgassert(body != nullptr);
    auto &genStmtList = body->getChildren()[0];
    splc_dbgassert(genStmtList != nullptr);
    recRegisterExprs(varList, varMap, stmtList, genStmtList);
}

void IRBuilder::recursiveParseAST(PtrAST astRoot)
{
    for (auto &child : astRoot->getChildren()) {
        if (isASTSymbolTypeOneOfThem(
                child->getSymbolType(), ASTSymbolType::ParseRoot,
                ASTSymbolType::TransUnit, ASTSymbolType::ExternDeclList)) {
            recursiveParseAST(child);
        }
        else if (child->getSymbolType() == ASTSymbolType::FuncDef) {
            parseFunction(child);
        }
        // TODO
        splc_error() << "unsupported type: " << child->getSymbolType()
                     << "\nat " << child->getLocation();
    }
}

void IRBuilder::parseAST(PtrAST astRoot)
{
    rootCtxt = astRoot->getASTContext();
    recursiveParseAST(astRoot);
}