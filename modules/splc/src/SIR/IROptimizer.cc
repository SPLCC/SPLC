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

DepKey IROptimizer::buildDependency(Ptr<IRFunction> func)
{
    DepNodeList allDepNodes;
    DepNodeMap nodeMap;
    DepNodeList inputs;
    DepNodeList outputs;
    using Type = DepNode::Type;

    for (auto &stmt : func->body) {
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
            auto lhs = insertOrReplace(nodeMap, stmt->op1, stmt, Type::Mid);
            auto rhs = findOrMake(nodeMap, stmt->op2, stmt, Type::Mid);
            DepNode::connect(rhs, lhs);
            allDepNodes.insert(allDepNodes.end(), {lhs, rhs});
            break;
        }
        case IRType::Return: {
            auto lhs = findOrMake(nodeMap, stmt->op1, stmt, Type::Output);
            allDepNodes.insert(allDepNodes.end(), {lhs});
            outputs.insert(outputs.end(), {lhs});
            break;
        }
        case IRType::Alloc: {
            auto lhs1 = findOrMake(nodeMap, stmt->op1, stmt, Type::Output);
            auto lhs2 = findOrMake(nodeMap, stmt->op2, stmt, Type::Output);
            allDepNodes.insert(allDepNodes.end(), {lhs1, lhs2});
            outputs.insert(outputs.end(), {lhs1, lhs2});
            break;
        }
        case IRType::PopCallArg: {
            auto node = insertOrReplace(nodeMap, stmt->op1, stmt, Type::Input);
            allDepNodes.push_back(node);
            inputs.push_back(node);
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
            inputs.insert(outputs.end(), {lhs});
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

    return {allDepNodes, nodeMap, inputs, outputs};
}

void recursiveColor(DepNode *node)
{
    // TODO: output
    node->setMarked();
    for (auto parent : node->parents) {
        recursiveColor(parent);
    }
}

void color(DepNodeList &inputs, DepNodeList &outputs)
{
    for (auto &input : inputs) {
        input->setMarked(); // retain expected behavior
    }

    for (auto &output : outputs) {
        recursiveColor(output.get());
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
        case IRType::Goto: {
            // Don't care
            newBody.push_back(stmt);
            break;
        }
        case IRType::BranchIf: {
            // Don't care
            newBody.push_back(stmt);
            break;
        }
        case IRType::Return: {
            // Don't care
            newBody.push_back(stmt);
            break;
        }
        case IRType::Alloc: {
            // Don't care
            newBody.push_back(stmt);
            break;
        }
        case IRType::PopCallArg: {
            // Don't care
            newBody.push_back(stmt);
            break;
        }
        case IRType::PushCallArg: {
            // Don't care
            newBody.push_back(stmt);
            break;
        }
        case IRType::InvokeFunc: {
            // Don't care
            newBody.push_back(stmt);
            break;
        }
        case IRType::Read: {
            // Don't care
            newBody.push_back(stmt);
            break;
        }
        case IRType::Write: {
            // Don't care
            newBody.push_back(stmt);
            break;
        }
        }
    }

    func->body = newBody;
}

void IROptimizer::constPropagation(Ptr<IRFunction> func)
{
    // TODO
}

void IROptimizer::optimizeArithmetic(Ptr<IRFunction> func)
{
    // TODO
}

void IROptimizer::optimizeFunction(Ptr<IRFunction> func)
{
    // TODO:
    removeUnusedStmts(func);
    constPropagation(func);
    optimizeArithmetic(func);
}

void IROptimizer::optimizeProgram(Ptr<IRProgram> prog)
{
    // TODO:
    for (auto &func : prog->funcMap) {
        optimizeFunction(func.second);
    }
}

} // namespace splc::SIR