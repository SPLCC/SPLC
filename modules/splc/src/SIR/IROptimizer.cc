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
        auto lhs = insertOrReplace(nodeMap, stmt->op1, stmt, Type::Mid);
        auto rhs = findOrMake(nodeMap, stmt->op2, stmt, Type::Mid);
        DepNode::connect(rhs, lhs);
        allDepNodes.insert(allDepNodes.end(), {lhs, rhs});
        break;
    }
    case IRType::Plus:
    case IRType::Minus:
    case IRType::Mul:
    case IRType::Div: {
        auto lhs = insertOrReplace(nodeMap, stmt->op1, stmt, Type::Mid);
        auto rhs1 = findOrMake(nodeMap, stmt->op2, stmt, Type::Mid);
        auto rhs2 = findOrMake(nodeMap, stmt->op3, stmt, Type::Mid);
        DepNode::connect(rhs1, lhs);
        DepNode::connect(rhs2, lhs);
        allDepNodes.insert(allDepNodes.end(), {lhs, rhs1, rhs2});
        break;
    }
    case IRType::Goto: {
        // Don't care
        break;
    }
    case IRType::BranchIf: {
        auto lhsPrev = findOrMake(nodeMap, stmt->op1, stmt, Type::Mid);
        auto rhsPrev = findOrMake(nodeMap, stmt->op2, stmt, Type::Mid);

        // change this
        auto lhsBranch =
            insertOrReplace(nodeMap, stmt->op1, stmt, Type::Output);
        auto rhsBranch =
            insertOrReplace(nodeMap, stmt->op2, stmt, Type::Output);

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
        auto lhs2 = findOrMake(nodeMap, stmt->op2, stmt, Type::Output);
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
        auto lhs = findOrMake(nodeMap, stmt->op1, stmt, Type::Input);
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
    auto &[allDepNodes, nodeMap, inputs, outputs] = key;

    for (auto it = func->body.begin(); it != func->body.end(); ++it) {
        examineStmt(key, func, it);
    }

    return key;
}

void recursiveColorParent(DepNode *node, bool colorChildren);

void recursiveColorChildren(DepNode *node)
{
    // using Type = DepNode::Type;

    // IRVec<DepNode *> stack;
    // IRSet<DepNode *> accessed;
    // stack.push_back(node);
    // while (!stack.empty()) {
    //     auto &top = stack.back();
    //     bool allFinished = true;

    //     if (top->children.empty()) {
    //         // end point
    //         if (top->type == Type::Output) {
    //             recursiveColorParent(top, false);
    //             top->setMarked();
    //         }
    //         accessed.insert(top);
    //         stack.pop_back();
    //         continue;
    //     }

    //     for (auto child : top->children) {
    //         if (accessed.contains(child))
    //             continue;
    //         allFinished = false;
    //         stack.push_back(child);
    //     }

    //     if (allFinished) {
    //         accessed.insert(top);
    //         stack.pop_back();
    //         continue;
    //     }
    // }

    // TODO(future): refactor all
    node->setMarked();
    for (auto &n : node->children) {
        recursiveColorChildren(n);
    }
}

void recursiveColorParent(DepNode *node, bool colorChildren)
{
    // TODO: output
    node->setMarked();
    for (auto parent : node->parents) {
        if (parent->isUnmarked()) {
            recursiveColorParent(parent, colorChildren);
        }
    }
    if (node->stmt != nullptr && node->stmt->isBranchIf() && colorChildren) {
        recursiveColorChildren(node);
    }
}

void color(DepNodeList &inputs, DepNodeList &outputs)
{
    for (auto &input : inputs) {
        input->setMarked(); // retain expected behavior
    }

    for (auto &output : outputs) {
        recursiveColorParent(output.get(), true);
    }
}

void IROptimizer::removeUnusedStmts(Ptr<IRFunction> func)
{
    using Type = DepNode::Type;

    auto [depNodes, nodeMap, inputs, outputs] = buildDependency(func);
    IRSet<IRStmt *> unusedStmts;

    // mark from output to input
    color(inputs, outputs);
    for (const auto &node : depNodes) {
        if (node->isUnmarked()) {
            unusedStmts.insert(node->getStmt());
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
            if (!unusedStmts.contains(stmt.get())) {
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
    if (node->var != nullptr && node->var->irVarType == IRVarType::Constant) {
        initials.push_back(node);
    }
    for (auto child : node->children) {
        getInitialConstants(initials, child);
    }
}

void constPropagate(DepNode *node)
{
    // TODO
    for (auto child : node->children) {
        if (child->stmt == nullptr)
            continue;
        switch (child->stmt->getIRType()) {
        case IRType::SetLabel:
        case IRType::FuncDecl: {
            break;
        }
        case IRType::Assign: {
            break;
        }
        case IRType::Plus:
        case IRType::Minus:
        case IRType::Mul:
        case IRType::Div:
        case IRType::AddrOf:
        case IRType::Deref:
        case IRType::CopyToAddr: {
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
    for (auto child : node->children) {
        constPropagate(child);
    }
}

void IROptimizer::constPropagate(Ptr<IRFunction> func)
{
    // TODO: basic const propagation
    using Type = DepNode::Type;

    auto [depNodes, nodeMap, inputs, outputs] = buildDependency(func);

    // traverse dependency graph for each constant variable
    IRVec<DepNode *> initials;

    for (auto &node : outputs) {
        getInitialConstants(initials, node.get());
    }

    for (auto initial : initials) {
        SIR::constPropagate(initial);
    }
}

void IROptimizer::optimizeArithmetic(Ptr<IRFunction> func)
{
    // TODO
}

void IROptimizer::optimizeFunction(Ptr<IRFunction> func)
{
    // TODO:
    // constPropagate(func);
    optimizeArithmetic(func);
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