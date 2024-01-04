#include "AST/Value.hh"
#include "AST/DerivedAST.hh"

namespace splc {

enum class ValueTy {
    // 基本类型
    Function,
    GlobalAlias,
    GlobalIFunc,
    GlobalVariable,
    BlockAddress,
    ConstantExpr,

    // 聚合类型
    ConstantArray,
    ConstantStruct,
    ConstantVector,

    // 常量数据
    UndefValue,
    ConstantAggregateZero,
    ConstantDataArray,
    ConstantDataVector,
    ConstantInt,
    ConstantFP,
    ConstantPointerNull,
    ConstantTokenNone,

    // 其他
    Argument,
    BasicBlock,
    MetadataAsValue,
    InlineAsm,

    // 内存相关
    MemoryUse,
    MemoryDef,
    MemoryPhi,

    // 指令类型
    Instruction,

    // 可以继续添加更多类型...
};



} // namespace splc