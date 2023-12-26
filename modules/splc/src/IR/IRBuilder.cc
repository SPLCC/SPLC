#include "IR/IRBuilder.hh"

using namespace splc;

void IRBuilder::recursiveParseAST(PtrAST parseRoot)
{
    if (parseRoot->getSymbolType() == ASTSymbolType::FuncDef) {
        registerFunction(parseRoot);
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
                splc_unreachable();
            }
        }
    }
}

void IRBuilder::registerFunction(PtrAST funcRoot)
{
    // Find function name (id)
    IRIDType funcID = IRBuilderHelper::recfindFuncID(funcRoot);
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
        SPLC_LOG_WARN(nullptr, false)
            << "established parameter mapping from " << p.first << " to "
            << p.second->getName();
    }

    function->paramMap.insert(params.begin(), params.end());
    function->varMap.insert(params.begin(), params.end());

    // Insert to funcMap
    funcMap.insert({funcID, function});

    // Register the body stmts
    PtrAST body = funcRoot->getChildren()[2];
    splc_dbgassert(body != nullptr);

    SPLC_LOG_DEBUG(nullptr, false) << "trying to register statement";
    recRegisterStmts(function->body, body);
}

void IRBuilder::recRegisterStmts(IRVec<PtrIRStmt> stmtList, PtrAST stmtRoot)
{
    SPLC_LOG_DEBUG(nullptr, false) << "dispatch: " << stmtRoot->getSymbolType();
    if (isASTSymbolTypeOneOfThem(stmtRoot->getSymbolType(),
                                 ASTSymbolType::GeneralStmtList,
                                 ASTSymbolType::CompStmt)) {
        for (auto &child : stmtRoot->getChildren()) {
            recRegisterStmts(stmtList, child);
        }
    }
    else if (stmtRoot->getSymbolType() == ASTSymbolType::Decl) {
        for (auto &child : stmtRoot->getChildren()) {
            recRegisterDeclVal(stmtList, child);
            // ASSIGN INITIAL VALUES, IF NONCONSTEXPR
        }
    }
    else if (stmtRoot->getSymbolType() == ASTSymbolType::Stmt) {
        PtrAST realStmt = stmtRoot->getChildren()[0];
        switch (realStmt->getSymbolType()) {
        case ASTSymbolType::CompStmt: {
            recRegisterStmts(stmtList, realStmt);
            break;
        }
        case ASTSymbolType::ExprStmt: {
            recRegisterExprs(stmtList, realStmt);
            break;
        }
        case ASTSymbolType::IterStmt: {
            recRegisterIterStmt(stmtList, realStmt);
            break;
        }
        case ASTSymbolType::SelStmt: {
            recRegisterSelStmt(stmtList, realStmt);
            break;
        }
        case ASTSymbolType::LabeledStmt: {
            splc_error() << "Not implement.\n";
            break;
        }
        case ASTSymbolType::JumpStmt: {
            recRegisterJumpStmt(stmtList, realStmt);
            break;
        }
        default: {
            splc_error();
        }
        }
    }
}


PtrIRVar IRBuilder::recRegisterExprs(IRVec<PtrIRStmt> stmtList,
                                         PtrAST exprRoot)
{
    if (exprRoot->getChildrenNum() == 1) {
        PtrAST child = exprRoot->getChildren()[0];
        switch (child->getSymbolType()) {
        case ASTSymbolType::ExprStmt:
        case ASTSymbolType::Expr: {
            return recRegisterExprs(stmtList, child);
        }
        case ASTSymbolType::CallExpr: {
            return recRegisterCallExpr(stmtList, child);
        }
        case ASTSymbolType::Constant: {
            return IRVar::createConstantVar(
                &tyCtxt.SInt32Ty, child->getChildren()[0]->getVariant());
        }
        case ASTSymbolType::ID: {
            auto it = varMap.find(child->getValue<IRIDType>());
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
        if (children[0]->getSymbolType() == ASTSymbolType::OpMinus) {
            PtrIRVar var = recRegisterExprs(stmtList, children[1]);
            var->val = -static_cast<ASTSIntType>(var->getValue<ASTUIntType>());
            return var;
        }
        else {
            splc_error() << "unary operations are unsupported except OpMinus";
        }
    }
    else if (exprRoot->getChildrenNum() == 3) {
        ASTSymbolType opType = exprRoot->getChildren()[1]->getSymbolType();
        PtrAST exprL = exprRoot->getChildren()[0];
        PtrAST exprR = exprRoot->getChildren()[2];

        PtrIRVar lhs = recRegisterExprs(stmtList, exprL);
        PtrIRVar rhs = recRegisterExprs(stmtList, exprR);

        if (opType == ASTSymbolType::OpAssign) {
            splc_dbgassert(exprL->getSymbolType() == ASTSymbolType::ID);
            stmtList.push_back(IRStmt::createAssignStmt(lhs, rhs));
            return lhs;
        }
        else if (isASTSymbolTypeOneOfThem(
                     opType, ASTSymbolType::OpPlus, ASTSymbolType::OpMinus,
                     ASTSymbolType::OpAstrk, ASTSymbolType::OpDiv)) {
            IRType arithmeticType;
            switch (opType) {
            case ASTSymbolType::OpPlus: {
                arithmeticType = IRType::Plus;
                break;
            }
            case ASTSymbolType::OpMinus: {
                arithmeticType = IRType::Minus;
                break;
            }
            case ASTSymbolType::OpAstrk: {
                arithmeticType = IRType::Mul;
                break;
            }
            case ASTSymbolType::OpDiv: {
                arithmeticType = IRType::Div;
                break;
            }
            default: {
                splc_error();
            }
                PtrIRVar res = getTmpVar();
                stmtList.push_back(IRStmt::createArithmeticStmt(arithmeticType,
                                                                res, lhs, rhs));
                return res;
            }
        }
        else if (isASTSymbolTypeOneOfThem(
                     opType, ASTSymbolType::OpLT, ASTSymbolType::OpLE,
                     ASTSymbolType::OpGT, ASTSymbolType::OpGE,
                     ASTSymbolType::OpEQ, ASTSymbolType::OpNE,
                     ASTSymbolType::OpAnd, ASTSymbolType::OpOr)) {
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

void IRBuilder::recRegisterCondExpr(IRVec<PtrIRStmt> stmtList, PtrAST stmtRoot,
                                    PtrIRVar lbt, PtrIRVar lbf)
{
    IRVec<PtrAST> children = stmtRoot->getChildren();
    if (children.size() == 2 &&
        children[0]->getSymbolType() == ASTSymbolType::OpNot) {
        recRegisterCondExpr(stmtList, children[1], lbf, lbt);
    }
    else if (children.size() == 3) {
        ASTSymbolType opType = children[1]->getSymbolType();
        PtrAST exprL = children[0];
        PtrAST exprR = children[2];

        if (isASTSymbolTypeOneOfThem(opType, ASTSymbolType::OpLT,
                                     ASTSymbolType::OpLE, ASTSymbolType::OpGT,
                                     ASTSymbolType::OpGE, ASTSymbolType::OpEQ,
                                     ASTSymbolType::OpNE)) {
            IRBranchType bType;
            switch (opType) {
            case ASTSymbolType::OpLT: {
                bType = IRBranchType::LT;
                break;
            }
            case ASTSymbolType::OpLE: {
                bType = IRBranchType::LE;
                break;
            }
            case ASTSymbolType::OpGT: {
                bType = IRBranchType::GT;
                break;
            }
            case ASTSymbolType::OpGE: {
                bType = IRBranchType::GE;
                break;
            }
            case ASTSymbolType::OpEQ: {
                bType = IRBranchType::EQ;
                break;
            }
            case ASTSymbolType::OpNE: {
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
        else if (opType == ASTSymbolType::OpAnd) {
            PtrIRVar lb = getTmpLabel();
            recRegisterCondExpr(stmtList, exprL, lb, lbf);
            stmtList.push_back(IRStmt::createLabelStmt(lb));
            recRegisterCondExpr(stmtList, exprR, lbt, lbf);
        }
        else if (opType == ASTSymbolType::OpOr) {
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

void IRBuilder::recRegisterIterStmt(IRVec<PtrIRStmt> stmtList, PtrAST stmtRoot)
{
    // CHECK: WHILE
    // KwdWhild Expr Stmt
    IRVec<PtrAST> children = stmtRoot->getChildren();
    splc_dbgassert(children[0]->getSymbolType() == ASTSymbolType::KwdWhile);

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

void IRBuilder::recRegisterSelStmt(IRVec<PtrIRStmt> stmtList, PtrAST stmtRoot)
{
    IRVec<PtrAST> children = stmtRoot->getChildren();
    splc_dbgassert(children[0]->getSymbolType() == ASTSymbolType::KwdIf);

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

    // PtrAST op = exprRoot->getChildren()[1];
    // switch (op->getSymbolType()) {
    // case ASTSymbolType::OpAnd:
    // case ASTSymbolType::OpOr: {
    //     // TODO
    //     break;
    // }
    // case ASTSymbolType::OpLT:
    // case ASTSymbolType::OpLE:
    // case ASTSymbolType::OpGT:
    // case ASTSymbolType::OpGE:
    // case ASTSymbolType::OpEQ:
    // case ASTSymbolType::OpNE: {
    //     // TODO
    //     break;
    // }
    // default:{
    //     splc_error();
    // }
}

void IRBuilder::recRegisterJumpStmt(IRVec<PtrIRStmt> stmtList, PtrAST stmtRoot)
{
    IRVec<PtrAST> children = stmtRoot->getChildren();
    splc_dbgassert(children[0]->getSymbolType() == ASTSymbolType::KwdReturn &&
                   children.size() == 2);
    PtrIRVar var = recRegisterExprs(stmtList, children[1]);
    stmtList.push_back(IRStmt::createReturnStmt(var));
}

PtrIRVar IRBuilder::getTmpLabel()
{
    return IRVar::createLabelVar("_lb_" + std::to_string(allocCnt++));
}

PtrIRVar IRBuilder::getTmpVar()
{
    return IRVar::createVariableVar("_var_", &tyCtxt.SInt32Ty);
}