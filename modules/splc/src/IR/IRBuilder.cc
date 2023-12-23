#include "IR/IRBuilder.hh"
#include "AST/AST.hh"
#include "IR/IR.hh"
#include <ranges>

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
                ASTSymbolType::ParamTypeList, ASTSymbolType::ParamList,
                ASTSymbolType::ParamDecltr, ASTSymbolType::Decltr,
                ASTSymbolType::DirDecltr)) {
            recfindFuncParam(vec, child);
        }
        else if (child->getSymbolType() == ASTSymbolType::DirFuncDecltr) {
            splc_dbgassert(child->getChildrenNum() == 2);
            recfindFuncParam(vec, child->getChildren()[1]);
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
    SPLC_LOG_WARN(nullptr, false) << "trying to find funcDecls in: " << funcID;
    auto &vec = rootCtxt->getSymbolList();
    bool tryFindDecl = false;
    for (auto &ent : std::views::reverse(vec)) {
        if (tryFindDecl && ent.second.symEntTy == SymEntryType::Variable) {
            const auto &id = ent.first;
            const auto val =
                ent.second.hasValue()
                    ? static_cast<int>(ent.second.getValue<ASTUIntType>())
                    : ASTValueType{};
            Ptr<IRVar> var =
                makeSharedPtr<IRVar>(id, IRVarType::Variable, &tyCtxt.SInt32Ty,
                                     val, ent.second.hasValue());
            varList.push_back(var);
            varMap.insert({id, var});
            SPLC_LOG_DEBUG(nullptr, false) << "pushed " << var->name;
        }
        else if (ent.second.symEntTy == SymEntryType::Function) {
            if (tryFindDecl) {
                break;
            }
            else if (ent.first == funcID) {
                tryFindDecl = true;
            }
        }
    }
}

void IRBuilder::recRegisterWhileLoop(IRVec<Ptr<IRVar>> &varList,
                                     IRMap<IRIDType, Ptr<IRVar>> &varMap,
                                     IRVec<Ptr<IRStmt>> &stmtList,
                                     PtrAST loopRoot)
{
    // TODO
}

void IRBuilder::recRegisterIfStmt(IRVec<Ptr<IRVar>> &varList,
                                  IRMap<IRIDType, Ptr<IRVar>> &varMap,
                                  IRVec<Ptr<IRStmt>> &stmtList, PtrAST loopRoot)
{
    // TODO
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
    splc_dbgassert(it != varMap.end()) << " cannot find id in varMap: " << id;

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
Ptr<IRVar> IRBuilder::recRegisterStmts(IRVec<Ptr<IRVar>> &varList,
                                       IRMap<IRIDType, Ptr<IRVar>> &varMap,
                                       IRVec<Ptr<IRStmt>> &stmtList,
                                       PtrAST stmtRoot)
{
    SPLC_LOG_DEBUG(nullptr, false) << "dispatch: " << stmtRoot->getSymbolType();
    if (isASTSymbolTypeOneOfThem(stmtRoot->getSymbolType(),
                                 ASTSymbolType::GeneralStmtList,
                                 ASTSymbolType::CompStmt)) {
        {
            SPLC_LOG_DEBUG(nullptr, false)
                << "passed varMap, search root type = "
                << stmtRoot->getSymbolType();
            for (auto &ent : varMap) {
                SPLC_LOG_DEBUG(nullptr, false) << "varMap ent: " << ent.first;
            }
        }
        for (auto &child : stmtRoot->getChildren()) {
            recRegisterStmts(varList, varMap, stmtList, child);
        }
    }
    else if (stmtRoot->getSymbolType() == ASTSymbolType::Decl) {
        for (auto &child : stmtRoot->getChildren()) {
            recRegisterDecl(varMap, stmtList, child);
            // ASSIGN INITIAL VALUES, IF NONCONSTEXPR
        }
    }
    else if (stmtRoot->getSymbolType() == ASTSymbolType::Stmt) {
        // TODO
    }
    else if (stmtRoot->getSymbolType() == ASTSymbolType::Expr) {
        // register expression
        if (stmtRoot->getChildrenNum() == 1) {
            // either constant or ID
            auto &child = stmtRoot->getChildren()[0];
            if (child->getSymbolType() == ASTSymbolType::ID) {
                auto it = varMap.find(child->getValue<IRIDType>());
                return it->second;
            }
            else {
                splc_dbgassert(child->getSymbolType() ==
                               ASTSymbolType::Constant);
                auto val = child->getChildren()[0]->getValue<ASTUIntType>();
                Ptr<IRVar> tmp = getTmpVar();
                tmp->val = val;
                tmp->isConst = true;
                return tmp;
            }
        }
        if (stmtRoot->getChildrenNum() == 2) {
            // ?
            splc_error();
        }
        else if (stmtRoot->getChildrenNum() == 3) {
            // either conditional or assignment
            auto &children = stmtRoot->getChildren();
            auto op = children[1]->getSymbolType();
            if (op == ASTSymbolType::OpAssign) {
            }
            else {
                switch (op) {
                case ASTSymbolType::OpPlus:
                case ASTSymbolType::OpMinus:
                case ASTSymbolType::OpAstrk:
                case ASTSymbolType::OpDiv: {
                    auto op1 =
                        recRegisterStmts(varList, varMap, stmtList, stmtRoot);
                    auto op2 =
                        recRegisterStmts(varList, varMap, stmtList, stmtRoot);
                    auto res = getTmpVar();
                    IRType irType;

                    switch (op) {
                    case ASTSymbolType::OpPlus:
                        irType = IRType::Plus;
                    case ASTSymbolType::OpMinus:
                        irType = IRType::Minus;
                    case ASTSymbolType::OpAstrk:
                        irType = IRType::Mul;
                    case ASTSymbolType::OpDiv:
                        irType = IRType::Div;
                    default:
                        splc_error();
                    }
                    Ptr<IRStmt> stmt{};
                }
                case ASTSymbolType::OpAssign: {
                }
                // case ASTSymbolType::OpLT:
                // case ASTSymbolType::OpLE:
                // case ASTSymbolType::OpGT:
                // case ASTSymbolType::OpGE:
                // case ASTSymbolType::OpEQ:
                // case ASTSymbolType::OpNE: {

                // }
                default:
                    splc_error();
                }
            }
        }
        else {
            splc_error();
            splc_unreachable();
        }
    }
}

void IRBuilder::parseFunction(PtrAST funcRoot)
{
    IRIDType funcID = recfindFuncID(funcRoot);
    IRVec<IRIDType> paramIDs;
    recfindFuncParam(paramIDs, funcRoot);
    IRVec<Type *> paramTys{paramIDs.size(), &tyCtxt.SInt32Ty};
    SPLC_LOG_WARN(nullptr, false) << "parsed func: " << funcID;
    for (auto &pid : paramIDs) {
        SPLC_LOG_DEBUG(nullptr, false)
            << "func: " << funcID << ", param = " << pid;
    }

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

    SPLC_LOG_WARN(nullptr, false) << "trying to register statement";
    recRegisterStmts(varList, varMap, stmtList, genStmtList);

    auto func = registerFunction(funcID, &tyCtxt.SInt32Ty, paramTys, paramIDs);
    func->functionBody = stmtList;
}

void IRBuilder::recursiveParseAST(PtrAST parseRoot)
{
    if (parseRoot->getSymbolType() == ASTSymbolType::FuncDef) {
        parseFunction(parseRoot);
    }
    else {
        for (auto &child : parseRoot->getChildren()) {
            if (isASTSymbolTypeOneOfThem(
                    child->getSymbolType(), ASTSymbolType::ParseRoot,
                    ASTSymbolType::TransUnit, ASTSymbolType::ExternDeclList,
                    ASTSymbolType::ExternDecl, ASTSymbolType::FuncDef)) {
                recursiveParseAST(child);
            }
            else {
                // TODO
                splc_error() << "unsupported type: " << child->getSymbolType()
                             << "\nat " << child->getLocation();
            }
        }
    }
}

void IRBuilder::parseAST(PtrAST astRoot)
{
    rootCtxt = astRoot->getASTContext();
    recursiveParseAST(astRoot);
}