#include "SIR/IROptimizer.hh"

namespace splc::SIR {

Ptr<DepNode> findOrMake(DepNodeMap &map, Ptr<IRVar> var, Ptr<IRStmt> stmt,
                        DepNode::Type type)
{
    auto it = map.find(var.get());
    if (it == map.end()) {
        auto node = DepNode::make(var, stmt, type);
        map.insert({var.get(), node});
        return node;
    }
    return it->second;
}

Ptr<DepNode> insertOrReplace(DepNodeMap &map, Ptr<IRVar> var, Ptr<IRStmt> stmt,
                             DepNode::Type type)
{
    auto node = DepNode::make(var, stmt, type);
    map.insert_or_assign(var.get(), node);
    return node;
}

void IROptimizer::examineStmt(DepKey &key, Ptr<IRFunction> func,
                              Ptr<IRStmt> &stmt)
{
    using Type = DepNode::Type;
    auto &[allDepNodes, nodeMap, inputs, outputs] = key;
    switch (stmt->getIRType()) {
    case IRType::SetLabel:
    case IRType::FuncDecl: {
        // Don't care
        break;
    }
    case IRType::Assign:
    case IRType::AddrOf:
    case IRType::Deref:
    case IRType::CopyToAddr: {
        auto rhs = findOrMake(nodeMap, stmt->op2, nullptr, Type::Mid);
        auto lhs = insertOrReplace(nodeMap, stmt->op1, stmt, Type::Mid);
        DepNode::connect(rhs, lhs);
        allDepNodes.insert(allDepNodes.end(), {rhs, lhs});
        // SPLC_LOG_DEBUG(nullptr, false)
        //     << "inserted assignment"
        //     << ", lhs: " << lhs->var->name << ", rhs: " << rhs->var->name;
        break;
    }
    case IRType::Plus:
    case IRType::Minus:
    case IRType::Mul:
    case IRType::Div: {
        auto rhs1 = findOrMake(nodeMap, stmt->op2, nullptr, Type::Mid);
        auto rhs2 = findOrMake(nodeMap, stmt->op3, nullptr, Type::Mid);
        auto lhs = insertOrReplace(nodeMap, stmt->op1, stmt, Type::Mid);
        DepNode::connect(rhs1, lhs);
        DepNode::connect(rhs2, lhs);
        allDepNodes.insert(allDepNodes.end(), {rhs1, rhs2, lhs});
        // SPLC_LOG_DEBUG(nullptr, false)
        //     << "inserted"
        //     << ", lhs: " << lhs->var->name << ", rhs1: " << rhs1->var->name
        //     << ", rhs2: " << rhs2->var->name;
        break;
    }
    case IRType::Goto: {
        // Don't care
        break;
    }
    case IRType::BranchIf: {
        auto rhsPrev = findOrMake(nodeMap, stmt->op2, nullptr, Type::Mid);
        auto lhsPrev = findOrMake(nodeMap, stmt->op1, nullptr, Type::Mid);

        // change this
        auto rhsBranch =
            insertOrReplace(nodeMap, stmt->op2, stmt, Type::Output);
        auto lhsBranch =
            insertOrReplace(nodeMap, stmt->op1, stmt, Type::Output);

        DepNode::connect(lhsPrev, lhsBranch);
        DepNode::connect(rhsPrev, rhsBranch);

        outputs.insert(outputs.end(), {lhsBranch, rhsBranch});
        allDepNodes.insert(allDepNodes.end(),
                           {lhsPrev, rhsPrev, lhsBranch, rhsBranch});
        break;
    }
    case IRType::Return: {
        auto lhs = findOrMake(nodeMap, stmt->op1, stmt, Type::Output);
        allDepNodes.push_back(lhs);
        outputs.push_back(lhs);
        break;
    }
    case IRType::Alloc: {
        auto lhs1 = insertOrReplace(nodeMap, stmt->op1, stmt, Type::Output);
        auto lhs2 = findOrMake(nodeMap, stmt->op2, nullptr, Type::Output);
        allDepNodes.insert(allDepNodes.end(), {lhs1, lhs2});
        outputs.insert(outputs.end(), {lhs1, lhs2});
        break;
    }
    case IRType::PopCallArg: {
        auto node = findOrMake(nodeMap, stmt->op1, stmt, Type::Output);
        allDepNodes.push_back(node);
        outputs.push_back(node);
        break;
    }
    case IRType::PushCallArg: {
        auto lhs = findOrMake(nodeMap, stmt->op1, stmt, Type::Output);
        allDepNodes.insert(allDepNodes.end(), {lhs});
        outputs.insert(outputs.end(), {lhs});
        break;
    }
    case IRType::InvokeFunc: {
        // Don't care
        break;
    }
    case IRType::Read: {
        auto lhs = insertOrReplace(nodeMap, stmt->op1, stmt, Type::Input);
        allDepNodes.insert(allDepNodes.end(), {lhs});
        inputs.insert(inputs.end(), {lhs});
        break;
    }
    case IRType::Write: {
        auto lhs = findOrMake(nodeMap, stmt->op1, stmt, Type::Output);
        allDepNodes.insert(allDepNodes.end(), {lhs});
        outputs.insert(outputs.end(), {lhs});
        break;
    }
    }
}

void IROptimizer::examineStmt(DepKey &key, Ptr<IRFunction> func,
                              decltype(func->body.begin()) it)
{
    examineStmt(key, func, *it);
}

void IROptimizer::searchBranchIf(DepKey &key, Ptr<IRFunction> func,
                                 decltype(func->body.begin()) it)
{
    auto &[allDepNodes, nodeMap, inputs, outputs] = key;
}

DepKey IROptimizer::buildDependency(Ptr<IRFunction> func)
{
    DepKey key;

    for (auto it = func->body.begin(); it != func->body.end(); ++it) {
        examineStmt(key, func, it);
    }

    return key;
}

void recursiveColor(DepNode *node)
{
    // SPLC_LOG_WARN(nullptr, false)
    //     << "processing node parents: " << node->var->getName();
    // for (auto parent : node->parents) {
    //     SPLC_LOG_DEBUG(nullptr, false)
    //         << "`-" << parent->var->getName() << ", " << parent->isMarked();
    // }
    // TODO: output
    node->setMarked();
    for (auto parent : node->parents) {
        if (parent->isUnmarked()) {
            recursiveColor(parent);
        }
    }
    for (auto child : node->children) {
        if (child->isUnmarked()) {
            recursiveColor(child);
        }
    }
}

void color(DepNodeList &inputs, DepNodeList &outputs)
{
    for (auto &output : outputs) {
        recursiveColor(output.get());
    }

    for (auto &input : inputs) {
        input->setMarked(); // retain expected behavior
    }
}

void IROptimizer::removeUnusedStmts(Ptr<IRFunction> func)
{
    using Type = DepNode::Type;

    auto [depNodes, nodeMap, inputs, outputs] = buildDependency(func);
    IRSet<IRVar *> usedVars;
    for (auto &depNode : outputs) {
        usedVars.insert(depNode->var);
    }

    IRSet<IRStmt *> usedStmts;

    // mark from output to input
    color(inputs, outputs);
    for (const auto &node : depNodes) {
        if (node->isMarked() || usedVars.contains(node->var)) {
            usedStmts.insert(node->getStmt());
        }
    }

    IRVec<Ptr<IRStmt>> newBody;

    for (auto &stmt : func->body) {
        switch (stmt->getIRType()) {
        case IRType::SetLabel:
        case IRType::FuncDecl: {
            // Don't care
            newBody.push_back(stmt);
            break;
        }
        case IRType::Assign:
        case IRType::AddrOf:
        case IRType::Deref:
        case IRType::CopyToAddr:
        case IRType::Plus:
        case IRType::Minus:
        case IRType::Mul:
        case IRType::Div: {
            if (usedStmts.contains(stmt.get())) {
                newBody.push_back(stmt);
            }
            break;
        }
        case IRType::Goto:
        case IRType::BranchIf:
        case IRType::Return:
        case IRType::Alloc:
        case IRType::PopCallArg:
        case IRType::PushCallArg:
        case IRType::InvokeFunc:
        case IRType::Read:
        case IRType::Write: {
            // Don't care
            newBody.push_back(stmt);
            break;
        }
        }
    }

    func->body = newBody;
}

void getInitialConstants(IRVec<DepNode *> &initials, DepNode *node)
{
    if (node->children.empty()) {
        initials.push_back(node);
    }
    for (auto parent : node->parents) {
        getInitialConstants(initials, parent);
    }
}

void constantPropagate(DepNode *node)
{
    // TODO
    if (node->stmt == nullptr)
        return;
    switch (node->stmt->getIRType()) {
    case IRType::SetLabel:
    case IRType::FuncDecl: {
        break;
    }
    case IRType::Assign: {
        splc_dbgassert(node->parents.size() == 1);
        for (auto parent : node->parents) {
            if (parent->stmt == nullptr)
                continue;
            if (parent->stmt->isArithmetic()) {
                SPLC_LOG_WARN(nullptr, false)
                    << "converting to arithmetic: " << node->var->getName();
                IRStmt::assignmentToArithmetic(node->stmt, parent->stmt);
            }
            else if (parent->stmt->isAssign()) {
                SPLC_LOG_WARN(nullptr, false)
                    << "concatenating assignments: " << node->var->getName();
                IRStmt::concatAssign(node->stmt, parent->stmt);
            }
        }
        break;
    }
    case IRType::Plus:
    case IRType::Minus:
    case IRType::Mul:
    case IRType::Div: {
        // const propagation
        splc_dbgassert(node->parents.size() == 2);
        SPLC_LOG_WARN(nullptr, false)
            << "propagating: " << node->var->getName();
        IRVar *v1 = node->stmt->op2.get();
        IRVar *v2 = node->stmt->op3.get();
        if (!(v1->isConst() && v2->isConst()))
            break;
        IRStmt::arithmeticToConstAssign(node->stmt, v1, v2);
        break;
    }
    case IRType::AddrOf:
    case IRType::Deref:
    case IRType::CopyToAddr: {
        // TODO
        break;
    }
    case IRType::Goto:
    case IRType::BranchIf:
    case IRType::Return:
    case IRType::Alloc:
    case IRType::PopCallArg:
    case IRType::PushCallArg:
    case IRType::InvokeFunc:
    case IRType::Read:
    case IRType::Write: {
        break;
    } break;
    }
}

void IROptimizer::constantPropagate(Ptr<IRFunction> func)
{
    // TODO: basic const propagation
    using Type = DepNode::Type;

    auto [depNodes, nodeMap, inputs, outputs] = buildDependency(func);

    // // traverse dependency graph for each constant variable
    // IRVec<DepNode *> initials;

    // for (auto &node : outputs) {
    //     getInitialConstants(initials, node.get());
    // }

    // for (auto node : initials) {
    //     SPLC_LOG_WARN(nullptr, false) << "initials: " <<
    //     node->var->getName();
    // }
    IRSet<DepNode *> visited;
    for (auto &initial : depNodes) {
        if (!visited.contains(initial.get())) {
            SPLC_LOG_DEBUG(nullptr, false) << "depnode at " << initial.get();
            SIR::constantPropagate(initial.get());
            visited.insert(initial.get());
        }
    }
}

void IROptimizer::optimizeArithmetic(Ptr<IRFunction> func)
{
    // TODO
}

void IROptimizer::optimizeFunction(Ptr<IRFunction> func)
{
    // TODO:
    constantPropagate(func);
    // optimizeArithmetic(func);
    removeUnusedStmts(func);
}

void IROptimizer::optimizeProgram(Ptr<IRProgram> prog)
{
    // TODO:
    for (auto &func : prog->funcMap) {
        optimizeFunction(func.second);
    }
}

} // namespace splc::SIR