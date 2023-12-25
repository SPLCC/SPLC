#include "IR/IROptimizer.hh"
#include "IR/IRBuilder.hh"

namespace splc {

void IROptimizer::optimize(IRBuilder &irBuilder) {
    // deadCodeElimination(irBuilder);
}


void IROptimizer::constFold(IRBuilder &irBuilder)
{
    for (auto &func : irBuilder.funcMap) {

        for (auto &stmt : func.second->functionBody) {
            // 确保指令是一个表达式（Plus, Minus, Mul, Div）
            if (stmt->irType != IRType::Plus && stmt->irType != IRType::Minus &&
                stmt->irType != IRType::Mul && stmt->irType != IRType::Div) {
                continue;
            }
            

            // 确保操作数都是常量
            if (!stmt->op2->isConst || !stmt->op3->isConst) {
                continue;
            }

            // 获取操作数的值
            auto val1 = stmt->op2->getValue<ASTUIntType>();
            auto val2 = stmt->op3->getValue<ASTUIntType>();

            ASTUIntType result;

            // 根据操作符类型进行计算
            switch (stmt->irType) {
            case IRType::Plus:
                result = val1 + val2;
                break;
            case IRType::Minus:
                result = val1 - val2;
                break;
            case IRType::Mul:
                result = val1 * val2;
                break;
            case IRType::Div:
                // 需要检查除零错误
                if (val2 != 0) {
                    result = val1 / val2;
                }
                else {
                    // 处理错误或保持原样
                    continue;
                }
                break;
            default:
                continue;
            }

            // 创建一个新的常量 IRVar
            auto newConstVar = makeSharedPtr<IRVar>("", IRVarType::Constant,
                                                    nullptr, result, true);
            irBuilder.writeIRStmt(std::cout, *stmt);

            // 替换原始语句
            // stmt->irType = IRType::Assign;
            // stmt->op2 = newConstVar; // 右值设置为新的常量
            // stmt->op1->isConst = true; 
            
            // Panding fix bugs
            auto newStmt = makeSharedPtr<IRStmt>(IRType::Assign, stmt->op1, newConstVar);
            newStmt->op1->isConst = true;
            stmt = newStmt;
        }
    }
}

void IROptimizer::deadCodeElimination(splc::IRBuilder &irBuilder)
{
    for (auto &func : irBuilder.funcMap) {
        std::set<IRIDType> usedVars;

        // 首先，收集所有被使用的变量
        for (auto &stmt : func.second->functionBody) {
            if (stmt->op1 && stmt->op1->irVarType == IRVarType::Variable) {
                usedVars.insert(stmt->op1->name);
            }
            if (stmt->op2 && stmt->op2->irVarType == IRVarType::Variable) {
                usedVars.insert(stmt->op2->name);
            }
            if (stmt->op3 && stmt->op3->irVarType == IRVarType::Variable) {
                usedVars.insert(stmt->op3->name);
            }
        }

        // 然后，移除没有被使用的变量赋值语句
        auto &stmtList = func.second->functionBody;
        for (auto it = stmtList.begin(); it != stmtList.end();) {
            if ((*it)->irType == IRType::Assign && (*it)->op1 &&
                (*it)->op1->irVarType == IRVarType::Variable &&
                usedVars.find((*it)->op1->name) == usedVars.end()) {
                // 该赋值语句是死代码，移除它
                it = stmtList.erase(it);
            }
            else {
                ++it;
            }
        }
    }
}

} // namespace splc