#include "IR/IRBuilder.hh"
#include "AST/AST.hh"
#include "IR/IR.hh"
#include <ranges>

using namespace splc;

Ptr<IRVar> IRBuilder::createVar(IRIDType id, IRVarType type, Type *ty)
{
    return makeSharedPtr<IRVar>(id, type, ty);
}

Ptr<IRVar> IRBuilder::getTmpVar()
{
    return createVar("tmp_" + std::to_string(allocCnt++), IRVarType::Variable,
                     &tyCtxt.SInt32Ty);
}

Ptr<IRVar> IRBuilder::getTmpLabel()
{
    return createVar("tmp_" + std::to_string(allocCnt++), IRVarType::Label,
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

void IRBuilder::recRegisterInitDecltr(IRVec<Ptr<IRVar>> &varList,
                                      IRMap<IRIDType, Ptr<IRVar>> &varMap,
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
        auto initExpr = root->getChildren()[2]->getChildren()[0];
        auto ret = recRegisterExprs(varList, varMap, stmtList, initExpr);
        Ptr<IRStmt> irStmt =
            makeSharedPtr<IRStmt>(IRType::Assign, it->second, ret);
        stmtList.push_back(irStmt);
    }
}

void IRBuilder::recRegisterDeclVal(IRVec<Ptr<IRVar>> &varList,
                                   IRMap<IRIDType, Ptr<IRVar>> &varMap,
                                   IRVec<Ptr<IRStmt>> &stmtList,
                                   PtrAST declRoot)
{
    for (auto &child : declRoot->getChildren()) {
        if (isASTSymbolTypeOneOfThem(child->getSymbolType(),
                                     ASTSymbolType::DirDecl)) {
            recRegisterDeclVal(varList, varMap, stmtList, child);
        }
        else if (isASTSymbolTypeOneOfThem(child->getSymbolType(),
                                          ASTSymbolType::InitDecltrList)) {
            for (auto &initDecltrs : child->getChildren()) {
                recRegisterInitDecltr(varList, varMap, stmtList, initDecltrs);
            }
        }
    }
}

// Now try to parse all expressions
Ptr<IRVar> IRBuilder::recRegisterExprs(IRVec<Ptr<IRVar>> &varList,
                                       IRMap<IRIDType, Ptr<IRVar>> &varMap,
                                       IRVec<Ptr<IRStmt>> &stmtList,
                                       PtrAST stmtRoot)
{
    // register expression
    if (stmtRoot->getSymbolType() == ASTSymbolType::CallExpr) {
        Ptr<IRVar> funcName = getTmpVar();
        funcName->isConst = true;
        funcName->name =
            stmtRoot->getChildren()[0]->getChildren()[0]->getValue<IRIDType>();

        SPLC_LOG_DEBUG(nullptr, false)
            << "evaluating CallExpr: " << funcName->name;

        auto tmp = getTmpVar();

        // try push all vars
        IRVec<Ptr<IRVar>> params;

        for (auto &arg : stmtRoot->getChildren()[1]->getChildren()) {
            params.push_back(recRegisterExprs(varList, varMap, stmtList, arg));
        }

        for (auto &p : std::views::reverse(params)) {
            SPLC_LOG_ERROR(nullptr, false)
                << "pushing call arg: " << p->getName();
            Ptr<IRStmt> argDecl = makeSharedPtr<IRStmt>(IRType::PushCallArg, p);
            stmtList.push_back(argDecl);
        }

        Ptr<IRStmt> stmt =
            makeSharedPtr<IRStmt>(IRType::InvokeFunc, tmp, funcName);
        stmtList.push_back(stmt);
        return tmp;
    }
    else if (stmtRoot->getChildrenNum() == 1) {
        // either constant or ID
        auto &child = stmtRoot->getChildren()[0];
        if (child->getSymbolType() == ASTSymbolType::ID) {
            auto it = varMap.find(child->getValue<IRIDType>());
            return it->second;
        }
        else {
            splc_dbgassert(child->getSymbolType() == ASTSymbolType::Constant);
            Ptr<IRVar> tmp = getTmpVar();
            tmp->val = child->getChildren()[0]->getVariant();
            tmp->isConst = true;
            varList.push_back(tmp);
            varMap.insert({tmp->name, tmp});
            return tmp;
        }
    }
    else if (stmtRoot->getChildrenNum() == 2) {
        splc_error() << "unary operations are unsupported";
    }
    else if (stmtRoot->getChildrenNum() == 3) {
        // either conditional or assignment
        auto &children = stmtRoot->getChildren();
        auto op = children[1]->getSymbolType();

        switch (op) {
        case ASTSymbolType::OpPlus:
        case ASTSymbolType::OpMinus:
        case ASTSymbolType::OpAstrk:
        case ASTSymbolType::OpDiv: {
            auto op1 = recRegisterExprs(varList, varMap, stmtList, children[0]);
            auto op2 = recRegisterExprs(varList, varMap, stmtList, children[2]);
            auto res = getTmpVar();
            IRType irType;
            switch (op) {
            case ASTSymbolType::OpPlus:
                irType = IRType::Plus;
                break;
            case ASTSymbolType::OpMinus:
                irType = IRType::Minus;
                break;
            case ASTSymbolType::OpAstrk:
                irType = IRType::Mul;
                break;
            case ASTSymbolType::OpDiv:
                irType = IRType::Div;
                break;
            default:
                splc_error();
            }
            varList.push_back(res);
            varMap.insert({res->name, res});
            Ptr<IRStmt> stmt = makeSharedPtr<IRStmt>(irType, res, op1, op2);
            stmtList.push_back(stmt);
            return res;
        }
        case ASTSymbolType::OpAssign: {
            auto op1 = recRegisterExprs(varList, varMap, stmtList, children[0]);
            auto op2 = recRegisterExprs(varList, varMap, stmtList, children[2]);
            Ptr<IRStmt> stmt = makeSharedPtr<IRStmt>(IRType::Assign, op1, op2);
            stmtList.push_back(stmt);
            SPLC_LOG_DEBUG(nullptr, false)
                << "pushed assignment: " << op1->getName() << " <- "
                << op2->getName();
            return op1;
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
    else {
        splc_error();
        splc_unreachable();
    }
}

// Now try to parse all expressions
void IRBuilder::recRegisterSingleStmt(IRVec<Ptr<IRVar>> &varList,
                                      IRMap<IRIDType, Ptr<IRVar>> &varMap,
                                      IRVec<Ptr<IRStmt>> &stmtList,
                                      PtrAST stmtRoot)
{
    auto realStmt = stmtRoot->getChildren()[0];

    switch (realStmt->getSymbolType()) {
    case ASTSymbolType::CompStmt: {
        recRegisterStmts(varList, varMap, stmtList, realStmt);
        break;
    }
    case ASTSymbolType::ExprStmt: {
        recRegisterExprs(varList, varMap, stmtList, realStmt->getChildren()[0]);
        break;
    }
    case ASTSymbolType::IterStmt: {
        // CHECK: WHILE
        splc_dbgassert(realStmt->getSymbolType() == ASTSymbolType::KwdWhile);
        auto label = getTmpLabel();

        Ptr<IRStmt> irStmt = makeSharedPtr<IRStmt>(IRType::Goto, label);
        // TODO
        stmtList.push_back(irStmt);
        break;
    }
    case ASTSymbolType::SelStmt: {
        // CHECK: IF/ELSE

        break;
    }
    case ASTSymbolType::JumpStmt: {
        // Jump Statement: return only
        // TODO: maybe support more

        splc_dbgassert(realStmt->getSymbolType() == ASTSymbolType::JumpStmt);
        auto &children = realStmt->getChildren();
        splc_dbgassert(children[1]->getSymbolType() == ASTSymbolType::Expr);
        Ptr<IRVar> var =
            recRegisterExprs(varList, varMap, stmtList, children[1]);
        Ptr<IRStmt> irStmt = makeSharedPtr<IRStmt>(IRType::Return, var);

        stmtList.push_back(irStmt);
        break;
    }
    default: {
        splc_error();
    }
    }
}

// Now try to parse all expressions
void IRBuilder::recRegisterStmts(IRVec<Ptr<IRVar>> &varList,
                                 IRMap<IRIDType, Ptr<IRVar>> &varMap,
                                 IRVec<Ptr<IRStmt>> &stmtList, PtrAST stmtRoot)
{
    SPLC_LOG_DEBUG(nullptr, false) << "dispatch: " << stmtRoot->getSymbolType();
    if (isASTSymbolTypeOneOfThem(stmtRoot->getSymbolType(),
                                 ASTSymbolType::GeneralStmtList,
                                 ASTSymbolType::CompStmt)) {
        for (auto &child : stmtRoot->getChildren()) {
            recRegisterStmts(varList, varMap, stmtList, child);
        }
    }
    else if (stmtRoot->getSymbolType() == ASTSymbolType::Decl) {
        for (auto &child : stmtRoot->getChildren()) {
            recRegisterDeclVal(varList, varMap, stmtList, child);
            // ASSIGN INITIAL VALUES, IF NONCONSTEXPR
        }
    }
    else if (stmtRoot->getSymbolType() == ASTSymbolType::Stmt) {
        recRegisterSingleStmt(varList, varMap, stmtList, stmtRoot);
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

    // register function first
    auto func = registerFunction(funcID, &tyCtxt.SInt32Ty, paramTys, paramIDs);

    // Now try to parse all declarations and reserve for expressions
    auto &varList = func->varList;
    auto &varMap = func->varMap;
    IRVec<Ptr<IRStmt>> stmtList; // just insert them linearly
    recfindFuncDecls(varList, varMap, funcRoot, funcID);

    // just find the function root first
    auto &body = funcRoot->getChildren()[2];
    splc_dbgassert(body != nullptr);
    auto &genStmtList = body->getChildren()[0];
    splc_dbgassert(genStmtList != nullptr);

    SPLC_LOG_WARN(nullptr, false) << "trying to register statement";
    recRegisterStmts(varList, varMap, stmtList, genStmtList);

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