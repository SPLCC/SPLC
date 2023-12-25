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

void IRBuilder::recRegisterIterStmt(IRVec<PtrIRStmt> stmtList, PtrAST stmtRoot)
{
    // CHECK: WHILE
    IRVec<PtrAST> children = stmtRoot->getChildren();
    splc_dbgassert(children[0]->getSymbolType() == ASTSymbolType::KwdWhile);
    PtrIRVar bLabel = getTmpLabel();
    PtrIRStmt bLabelStmt = IRStmt::createLabelStmt(bLabel);
    PtrIRVar eLabel = getTmpLabel();
    PtrIRStmt eLabelStmt = IRStmt::createLabelStmt(eLabel);

    // KwdWhild Expr Stmt
    recRegisterExprs(stmtList, children[1]);
    // TODO
}

PtrIRVar IRBuilder::getTmpLabel()
{
    return IRVar::createLabelVar("_lb_" + std::to_string(allocCnt++));
}

PtrIRVar IRBuilder::getTmpVar()
{
    return IRVar::createVariableVar("_var_", &tyCtxt.SInt32Ty);
}