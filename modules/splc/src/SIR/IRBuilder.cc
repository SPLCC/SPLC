#include "SIR/IRBuilder.hh"
#include "SIR/IR.hh"

using namespace splc;
using namespace splc::SIR;

void IRBuilder::parseAST(PtrAST parseRoot)
{
    if (parseRoot->isFuncDef()) {
        registerFunction(parseRoot);
    }
    else {
        for (auto &child : parseRoot->getChildren()) {
            if (child->isSymTypeOneOf(
                    ASTSymType::ParseRoot, ASTSymType::TransUnit,
                    ASTSymType::ExternDeclList, ASTSymType::ExternDecl,
                    ASTSymType::FuncDef)) {
                parseAST(child);
            }
            else {
                // TODO
                splc_error() << "unsupported type: " << child->getSymType()
                             << "\nat " << child->getLocation();
                splc_unreachable();
            }
        }
    }
}

void IRBuilder::registerFunction(PtrAST funcRoot)
{
    // Find function name (id)
    IRIDType funcID = funcRoot->getChildren()[1]
                          ->getChildren()[0]
                          ->getChildren()[0]
                          ->getConstVal<IRIDType>();

    // assume SInt32Ty
    Ptr<IRFunction> function = IRFunction::create(funcID, &tyCtxt.SInt32Ty);

    // Find all params
    IRVec<IRIDType> paramIDs = IRBuilderHelper::recfindFuncParam(funcRoot);

    IRVec<IRPair<IRIDType, PtrIRVar>> params;
    params.reserve(paramIDs.size());

    // TODO (future): support more type
    for (auto &pid : paramIDs) {
        params.emplace_back(pid,
                            IRVar::createVariableVar(pid, &tyCtxt.SInt32Ty));
    }

    for (auto &p : params) {
        function->paramList.push_back(p.second);
    }

    function->varMap.insert(params.begin(), params.end());
    varMap.insert(params.begin(), params.end());

    // Insert to funcMap
    funcMap.insert({funcID, function});
    funcVarMap.insert(
        {funcID, IRVar::createFunctionVar(funcID, &tyCtxt.SInt32Ty)});

    // Register the body stmts
    PtrAST body = funcRoot->getChildren()[2];
    splc_dbgassert(body != nullptr);

    SPLC_LOG_DEBUG(nullptr, false) << "trying to register statement";
    recRegisterStmts(function->body, body);

    // function->varList.insert(function->varList.end(), varList.begin(),
    // varList.end()); varList.clear(); for (const auto& pair : funcMap) {
    //     function->varMap.insert_or_assign(pair.first, pair.second);
    // }
    // varMap.clear();
}

PtrIRVar IRBuilder::recRegisterExprs(IRVec<PtrIRStmt> &stmtList,
                                     PtrAST exprRoot)
{
    if (exprRoot->isCallExpr()) {
        return recRegisterCallExpr(stmtList, exprRoot);
    }

    if (exprRoot->getChildrenNum() == 1) {
        PtrAST child = exprRoot->getChildren()[0];
        switch (child->getSymType()) {
        case ASTSymType::ExprStmt:
        case ASTSymType::Expr:
        case ASTSymType::CallExpr: {
            return recRegisterExprs(stmtList, child);
        }
        case ASTSymType::Constant: {
            return IRVar::createConstantVar(
                &tyCtxt.SInt32Ty, child->getChildren()[0]->getVariant());
        }
        case ASTSymType::ID: {
            auto it = varMap.find(child->getConstVal<IRIDType>());
            splc_dbgassert(it != varMap.end());
            return it->second;
        }
        default: {
            splc_error();
        }
        }
    }
    else if (exprRoot->getChildrenNum() == 2) {
        IRVec<PtrAST> children = exprRoot->getChildren();
        if (children[0]->isOpMinus()) {
            PtrIRVar var = recRegisterExprs(stmtList, children[1]);
            var->val = -static_cast<ASTSIntType>(var->getValue<ASTUIntType>());
            return var;
        }
        else {
            splc_error() << "unary operations are unsupported except OpMinus";
        }
    }
    else if (exprRoot->getChildrenNum() == 3) {
        ASTSymType opType = exprRoot->getChildren()[1]->getSymType();
        PtrAST exprL = exprRoot->getChildren()[0];
        PtrAST exprR = exprRoot->getChildren()[2];

        PtrIRVar lhs = recRegisterExprs(stmtList, exprL);
        PtrIRVar rhs = recRegisterExprs(stmtList, exprR);

        if (opType == ASTSymType::OpAssign) {
            splc_dbgassert(exprL->getChildren()[0]->getSymType() ==
                           ASTSymType::ID);
            stmtList.push_back(IRStmt::createAssignStmt(lhs, rhs));
            return lhs;
        }
        else if (isASTSymbolTypeOneOf(opType, ASTSymType::OpPlus,
                                      ASTSymType::OpMinus, ASTSymType::OpAstrk,
                                      ASTSymType::OpDiv)) {
            IRType arithmeticType;
            switch (opType) {
            case ASTSymType::OpPlus: {
                arithmeticType = IRType::Plus;
                break;
            }
            case ASTSymType::OpMinus: {
                arithmeticType = IRType::Minus;
                break;
            }
            case ASTSymType::OpAstrk: {
                arithmeticType = IRType::Mul;
                break;
            }
            case ASTSymType::OpDiv: {
                arithmeticType = IRType::Div;
                break;
            }
            default: {
                splc_error();
            }
            }
            PtrIRVar res = getTmpVar();
            stmtList.push_back(
                IRStmt::createArithmeticStmt(arithmeticType, res, lhs, rhs));
            return res;
        }
        else if (isASTSymbolTypeOneOf(
                     opType, ASTSymType::OpLT, ASTSymType::OpLE,
                     ASTSymType::OpGT, ASTSymType::OpGE, ASTSymType::OpEQ,
                     ASTSymType::OpNE, ASTSymType::OpAnd, ASTSymType::OpOr)) {
            PtrIRVar lb1 = getTmpLabel();
            PtrIRVar lb2 = getTmpLabel();

            // TODO: optimize
            // Treat True as 1 and False as 0
            PtrIRVar condResVar = getTmpVar();
            stmtList.push_back(IRStmt::createAssignStmt(
                condResVar, IRVar::createConstantVar(&tyCtxt.SInt32Ty, 0)));
            recRegisterCondExpr(stmtList, exprRoot, lb1, lb2);
            stmtList.push_back(IRStmt::createLabelStmt(lb1));
            stmtList.push_back(IRStmt::createAssignStmt(
                condResVar, IRVar::createConstantVar(&tyCtxt.SInt32Ty, 1)));
            stmtList.push_back(IRStmt::createLabelStmt(lb2));
            return condResVar;
        }
        else {
            splc_error();
        }
    }
    else {
        splc_error() << "Not implement triple operators or other.\n";
    }
}

void IRBuilder::recRegisterCondExpr(IRVec<PtrIRStmt> &stmtList, PtrAST stmtRoot,
                                    PtrIRVar lbt, PtrIRVar lbf)
{
    IRVec<PtrAST> children = stmtRoot->getChildren();
    if (children.size() == 2 &&
        children[0]->getSymType() == ASTSymType::OpNot) {
        recRegisterCondExpr(stmtList, children[1], lbf, lbt);
    }
    else if (children.size() == 3) {
        ASTSymType opType = children[1]->getSymType();
        PtrAST exprL = children[0];
        PtrAST exprR = children[2];

        if (isASTSymbolTypeOneOf(opType, ASTSymType::OpLT, ASTSymType::OpLE,
                                 ASTSymType::OpGT, ASTSymType::OpGE,
                                 ASTSymType::OpEQ, ASTSymType::OpNE)) {
            IRBranchType bType;
            switch (opType) {
            case ASTSymType::OpLT: {
                bType = IRBranchType::LT;
                break;
            }
            case ASTSymType::OpLE: {
                bType = IRBranchType::LE;
                break;
            }
            case ASTSymType::OpGT: {
                bType = IRBranchType::GT;
                break;
            }
            case ASTSymType::OpGE: {
                bType = IRBranchType::GE;
                break;
            }
            case ASTSymType::OpEQ: {
                bType = IRBranchType::EQ;
                break;
            }
            case ASTSymType::OpNE: {
                bType = IRBranchType::NE;
                break;
            }
            default: {
            }
            }

            PtrIRVar lhs = recRegisterExprs(stmtList, exprL);
            PtrIRVar rhs = recRegisterExprs(stmtList, exprR);

            stmtList.push_back(
                IRStmt::createBranchIfStmt(bType, lhs, rhs, lbt));
            stmtList.push_back(IRStmt::createGotoStmt(lbf));
        }
        else if (opType == ASTSymType::OpAnd) {
            PtrIRVar lb = getTmpLabel();
            recRegisterCondExpr(stmtList, exprL, lb, lbf);
            stmtList.push_back(IRStmt::createLabelStmt(lb));
            recRegisterCondExpr(stmtList, exprR, lbt, lbf);
        }
        else if (opType == ASTSymType::OpOr) {
            PtrIRVar lb = getTmpLabel();
            recRegisterCondExpr(stmtList, exprL, lbt, lb);
            stmtList.push_back(IRStmt::createLabelStmt(lb));
            recRegisterCondExpr(stmtList, exprR, lbt, lbf);
        }
        else {
            splc_error();
        }
    }
}

PtrIRVar IRBuilder::recRegisterCallExpr(IRVec<PtrIRStmt> &stmtList,
                                        PtrAST exprRoot)
{
    IRIDType funcID =
        exprRoot->getChildren()[0]->getChildren()[0]->getConstVal<IRIDType>();

    PtrAST argAST = exprRoot->getChildren()[1];
    SPLC_LOG_DEBUG(nullptr, false) << "evaluating CallExpr: " << funcID;

    // Read or Write function call
    if (funcID == "write") {
        SPLC_LOG_DEBUG(nullptr, false) << "Write to terminal\n";
        PtrIRVar arg = recRegisterExprs(stmtList, argAST->getChildren()[0]);
        stmtList.push_back(IRStmt::createWriteStmt(arg));
        return nullptr; // Nothing to return
    }
    else if (funcID == "read") {
        SPLC_LOG_DEBUG(nullptr, false) << "Read from input\n";
        PtrIRVar res = getTmpVar();
        stmtList.push_back(IRStmt::createReadStmt(res));
        return res;
    }

    auto it = funcVarMap.find(funcID);
    splc_dbgassert(it != funcVarMap.end());
    PtrIRVar funcVar = it->second;

    // normal function call
    PtrIRVar res = getTmpVar();

    for (auto &arg : argAST->getChildren()) {
        PtrIRVar argVar = recRegisterExprs(stmtList, arg);
        SPLC_LOG_DEBUG(nullptr, false)
            << "pushing call arg: " << argVar->getName();
        stmtList.push_back(IRStmt::createPushCallArgStmt(argVar));
    }

    // CALL stmt
    stmtList.push_back(IRStmt::createInvokeFuncStmt(res, funcVar));
    return res;
}

void IRBuilder::recRegisterStmts(IRVec<PtrIRStmt> &stmtList, PtrAST stmtRoot)
{
    SPLC_LOG_DEBUG(nullptr, false) << "dispatch: " << stmtRoot->getSymType();
    if (isASTSymbolTypeOneOf(stmtRoot->getSymType(),
                             ASTSymType::GeneralStmtList,
                             ASTSymType::CompStmt)) {
        for (auto &child : stmtRoot->getChildren()) {
            recRegisterStmts(stmtList, child);
        }
    }
    else if (stmtRoot->getSymType() == ASTSymType::Decl) {
        // ASSIGN INITIAL VALUES, IF NONCONSTEXPR
        recRegisterDeclVar(stmtList, stmtRoot);
    }
    else if (stmtRoot->isStmt()) {
        PtrAST realStmt = stmtRoot->getChildren()[0];
        switch (realStmt->getSymType()) {
        case ASTSymType::CompStmt: {
            recRegisterStmts(stmtList, realStmt);
            break;
        }
        case ASTSymType::ExprStmt: {
            recRegisterExprs(stmtList, realStmt);
            break;
        }
        case ASTSymType::IterStmt: {
            recRegisterIterStmt(stmtList, realStmt);
            break;
        }
        case ASTSymType::SelStmt: {
            recRegisterSelStmt(stmtList, realStmt);
            break;
        }
        case ASTSymType::LabeledStmt: {
            splc_error() << "Not implement.\n";
            break;
        }
        case ASTSymType::JumpStmt: {
            recRegisterJumpStmt(stmtList, realStmt);
            break;
        }
        default: {
            splc_error();
        }
        }
    }
}

void IRBuilder::recRegisterIterStmt(IRVec<PtrIRStmt> &stmtList, PtrAST stmtRoot)
{
    // CHECK: WHILE
    // KwdWhile Expr Stmt
    IRVec<PtrAST> children = stmtRoot->getChildren();
    splc_dbgassert(children[0]->getSymType() == ASTSymType::KwdWhile);

    // TODO: reverse op to optimize
    PtrIRVar lb1 = getTmpLabel();
    PtrIRStmt lbSt1 = IRStmt::createLabelStmt(lb1);
    PtrIRVar lb2 = getTmpLabel();
    PtrIRStmt lbSt2 = IRStmt::createLabelStmt(lb2);
    PtrIRVar lb3 = getTmpLabel();
    PtrIRStmt lbSt3 = IRStmt::createLabelStmt(lb3);

    stmtList.push_back(lbSt1);

    PtrAST exprRoot = children[1];
    recRegisterCondExpr(stmtList, exprRoot, lb2, lb3);

    stmtList.push_back(lbSt2);
    PtrAST body = children[2];
    recRegisterStmts(stmtList, body);
    stmtList.push_back(IRStmt::createGotoStmt(lb1));

    stmtList.push_back(lbSt3);
}

void IRBuilder::recRegisterSelStmt(IRVec<PtrIRStmt> &stmtList, PtrAST stmtRoot)
{
    IRVec<PtrAST> children = stmtRoot->getChildren();
    splc_dbgassert(children[0]->getSymType() == ASTSymType::KwdIf);

    // TODO: reverse op to optimize
    PtrIRVar lb1 = getTmpLabel();
    PtrIRStmt lbSt1 = IRStmt::createLabelStmt(lb1);
    PtrIRVar lb2 = getTmpLabel();
    PtrIRStmt lbSt2 = IRStmt::createLabelStmt(lb2);

    recRegisterCondExpr(stmtList, children[1], lb1, lb2);
    stmtList.push_back(lbSt1);
    recRegisterStmts(stmtList, children[2]);

    if (children.size() == 3) {
        // KwdIf Expr Stmt
        stmtList.push_back(lbSt2);
    }
    else if (children.size() == 5) {
        // KwdIf Expr Stmt1 KwdElse Stmt2
        PtrIRVar lb3 = getTmpLabel();
        PtrIRStmt lbSt3 = IRStmt::createLabelStmt(lb3);
        stmtList.push_back(IRStmt::createGotoStmt(lb3));
        stmtList.push_back(lbSt2);

        recRegisterStmts(stmtList, children[4]);
        stmtList.push_back(lbSt3);
    }
    else {
        splc_error();
    }
}

void IRBuilder::recRegisterJumpStmt(IRVec<PtrIRStmt> &stmtList, PtrAST stmtRoot)
{
    IRVec<PtrAST> children = stmtRoot->getChildren();
    splc_dbgassert(children[0]->getSymType() == ASTSymType::KwdReturn &&
                   children.size() == 2);
    PtrIRVar var = recRegisterExprs(stmtList, children[1]);
    stmtList.push_back(IRStmt::createReturnStmt(var));
}

void IRBuilder::recRegisterDeclVar(IRVec<PtrIRStmt> &stmtList, PtrAST declRoot)
{
    // TODO
    if (isASTSymbolTypeOneOf(declRoot->getSymType(), ASTSymType::Decl)) {
        recRegisterDeclVar(stmtList, declRoot->getChildren()[0]);
    }
    else if (declRoot->getSymType() == ASTSymType::DirDecl) {
        splc_dbgassert(declRoot->getChildrenNum() == 2);
        for (auto &initDecltr : declRoot->getChildren()[1]->getChildren()) {
            PtrAST decltr = initDecltr->getChildren()[0];

            if (decltr->getChildren()[0]->getSymType() ==
                ASTSymType::DirDecltr) {
                IRIDType id = decltr->getChildren()[0]
                                  ->getChildren()[0]
                                  ->getConstVal<IRIDType>();
                auto it = varMap.find(id);
                splc_dbgassert(it == varMap.end())
                    << "redefinition of id in varMap: " << id;
                PtrIRVar var = IRVar::createVariableVar(id, &tyCtxt.SInt32Ty);

                varList.push_back(var);
                varMap.insert({id, var});
                SPLC_LOG_DEBUG(nullptr, false)
                    << "defined id in varMap: " << id;

                // Process initializer, if any
                if (initDecltr->getChildrenNum() == 3) {
                    PtrIRVar init = recRegisterExprs(
                        stmtList, initDecltr->getChildren()[2]);
                    stmtList.push_back(IRStmt::createAssignStmt(var, init));
                }
            }
            else {
                splc_error();
            }
        }
    }
}

PtrIRVar IRBuilder::getTmpLabel()
{
    return IRVar::createLabelVar("lb_" + std::to_string(allocCnt++));
}

PtrIRVar IRBuilder::getTmpVar()
{
    return IRVar::createVariableVar("tmp_" + std::to_string(allocCnt++),
                                    &tyCtxt.SInt32Ty);
}
