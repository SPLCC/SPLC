#include "IR/IROptimizer.hh"
#include "IR/IRBuilder.hh"

namespace splc {

void IROptimizer::optimize(IRBuilder &irBuilder) {
    // TODO: Pending to finish
    // constantFold(irBuilder);
    // deadCodeElimination(irBuilder);
}


void IROptimizer::constantFold(IRBuilder &irBuilder)
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

void IROptimizer::deadCodeElimination(splc::IRBuilder &irBuilder) {
    for (auto &func : irBuilder.funcMap) {
        std::set<IRIDType> usedVars;
        std::map<IRIDType, std::vector<Ptr<IRStmt>>::iterator> lastAssign;

        auto &stmtList = func.second->functionBody;
        // 逆向遍历，确定每个变量的使用情况
        for (auto it = stmtList.rbegin(); it != stmtList.rend(); ++it) {
            auto &stmt = *it;

            // 检查赋值语句的右值，如果是变量，则标记为已使用
            if (stmt->op2 && stmt->op2->irVarType == IRVarType::Variable) {
                usedVars.insert(stmt->op2->name);
            }
            if (stmt->op3 && stmt->op3->irVarType == IRVarType::Variable) {
                usedVars.insert(stmt->op3->name);
            }
            // 对于赋值语句，记录最后一次赋值的位置
            if (stmt->irType == IRType::Assign && 
                stmt->op1->irVarType == IRVarType::Variable) {
                if (usedVars.find(stmt->op1->name) == usedVars.end()) {
                    // 如果赋值后未使用，则记录这个位置
                    
                    lastAssign[stmt->op1->name] = (std::next(it)).base();
                } else {
                    // 如果变量被使用，则从记录中移除
                    lastAssign.erase(stmt->op1->name);
                }
            }
        }

        // 移除未使用的赋值
        for (const auto &assign : lastAssign) {
            stmtList.erase(assign.second);
        }
    }
}




} // namespace splc