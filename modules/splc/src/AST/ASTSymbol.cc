#include "AST/ASTSymbol.hh"
#include "Core/Utils/ControlSequence.hh"

namespace splc {

namespace internal {

using utils::logging::ControlSeq;

// clang-format off
/// Automatically generated by GNU Bison
#ifdef YY_NULLPTR
#undef YY_NULLPTR
#endif
#define YY_NULLPTR nullptr

// YYTNAME[SYMBOL-NUM] -- String name of the symbol SYMBOL-NUM.
// First, the terminals, then, starting at \a YYNTOKENS, nonterminals.
const char*
const yytname_[] =
{
"\"end of file\"", "error", "\"invalid token\"", "KwdAuto", "KwdExtern",
"KwdRegister", "KwdStatic", "KwdTypedef", "KwdConst", "KwdRestrict",
"KwdVolatile", "KwdInline", "VoidTy", "CharTy", "ShortTy", "IntTy",
"SignedTy", "UnsignedTy", "LongTy", "FloatTy", "DoubleTy", "KwdEnum",
"KwdStruct", "KwdUnion", "KwdIf", "KwdElse", "KwdSwitch", "KwdWhile",
"KwdFor", "KwdDo", "KwdDefault", "KwdCase", "KwdGoto", "KwdContinue",
"KwdBreak", "KwdReturn", "ID", "TypedefID", "OpAssign", "OpMulAssign",
"OpDivAssign", "OpModAssign", "OpPlusAssign", "OpMinusAssign",
"OpLShiftAssign", "OpRShiftAssign", "OpBAndAssign", "OpBXorAssign",
"OpBOrAssign", "OpAnd", "OpOr", "OpNot", "OpLT", "OpLE", "OpGT", "OpGE",
"OpNE", "OpEQ", "OpQMark", "OpColon", "OpLShift", "OpRShift", "OpBAnd",
"OpBOr", "OpBNot", "OpBXor", "OpDPlus", "OpDMinus", "OpPlus", "OpMinus",
"OpAstrk", "OpDiv", "OpMod", "OpDot", "OpRArrow", "OpSizeOf", "OpLSB",
"OpRSB", "OpComma", "OpEllipsis", "PSemi", "PLC", "PRC", "PLP", "PRP",
"UIntLiteral", "SIntLiteral", "FloatLiteral", "CharLiteral", "StrUnit",
"SubscriptExpr", "CallExpr", "AccessExpr", "ExplicitCastExpr", "ImplicitCastExpr",
"AddrOfExpr", "DerefExpr", "SizeOfExpr", "KwdThen", "DecltrPrec",
"FuncDeclPrec", "OpUnaryPrec", "PLParen", "PRParen", "PLSBracket",
"PRSBracket", "$accept", "ParseRoot", "TransUnit",
"ExternDeclList", "ExternDecl", "DeclSpec", "StorageSpec",
"SpecQualList", "TypeSpec", "FuncSpec", "TypeQual", "TypeName",
"BuiltinTypeSpec", "AbsDecltr", "DirAbsDecltr", "StructOrUnionSpec",
"StructOrUnion", "StructDeclBody", "StructDeclList", "StructDecl",
"StructDecltrList", "StructDecltr", "EnumSpec", "EnumBody",
"EnumeratorList", "Enumerator", "EnumConst", "Decltr", "DirDecltr",
"WrappedDirDecltr", "TypeQualList", "Decl", "DirDecl", "InitDecltrList",
"InitDecltr", "Initializer", "InitializerList", "Designation",
"DesignatorList", "Designator", "FuncDef", "FuncProto", "FuncDecltr",
"DirFuncDecltr", "DirDecltrForFunc", "ParamTypeList", "ParamList",
"ParamDecltr", "CompStmt", "GeneralStmtList", "Stmt", "ExprStmt",
"SelStmt", "LabeledStmt", "JumpStmt", "IterStmt", "ForLoopBody",
"ConstExpr", "Constant", "PrimaryExpr", "PostfixExpr", "MemberAccessOp",
"UnaryExpr", "UnaryArithOp", "CastExpr", "MulExpr", "MulOp", "DivOp",
"AddExpr", "AddOp", "ShiftExpr", "ShiftOp", "RelExpr", "RelOp",
"EqualityExpr", "EqualityOp", "OpBAndExpr", "OpBXorExpr", "OpBOrExpr",
"LogicalOpAndExpr", "LogicalOpOrExpr", "CondExpr", "AssignExpr",
"AssignOp", "Expr", "InitExpr", "ArgList", "StringLiteral", "IDWrapper", YY_NULLPTR
};
// clang-format on

std::string_view getSymbolName(ASTSymType symbol) noexcept
{
    return {yytname_[static_cast<int>(symbol)]};
}

std::ostream &printSymbolConsoleTraits(std::ostream &os,
                                       SPLSymType symbol) noexcept
{
    switch (symbol) {
    case SPLSymType::YYNTOKENS:
    case SPLSymType::YYEMPTY:
    case SPLSymType::YYEOF:
    case SPLSymType::YYerror:
    case SPLSymType::YYUNDEF: {
        os << ControlSeq::Bold << ControlSeq::Red;
        break;
    }
    case SPLSymType::KwdAuto:
    case SPLSymType::KwdExtern:
    case SPLSymType::KwdRegister:
    case SPLSymType::KwdStatic:
    case SPLSymType::KwdTypedef:
    case SPLSymType::KwdConst:
    case SPLSymType::KwdRestrict:
    case SPLSymType::KwdVolatile:
    case SPLSymType::KwdInline:
    case SPLSymType::VoidTy:
    case SPLSymType::CharTy:
    case SPLSymType::ShortTy:
    case SPLSymType::IntTy:
    case SPLSymType::SignedTy:
    case SPLSymType::UnsignedTy:
    case SPLSymType::LongTy:
    case SPLSymType::FloatTy:
    case SPLSymType::DoubleTy: {
        os << ControlSeq::Bold << ControlSeq::Blue;
        break;
    }
    case SPLSymType::KwdEnum:
    case SPLSymType::KwdStruct:
    case SPLSymType::KwdUnion:
    case SPLSymType::KwdIf:
    case SPLSymType::KwdElse:
    case SPLSymType::KwdSwitch:
    case SPLSymType::KwdWhile:
    case SPLSymType::KwdFor:
    case SPLSymType::KwdDo:
    case SPLSymType::KwdDefault:
    case SPLSymType::KwdCase:
    case SPLSymType::KwdGoto:
    case SPLSymType::KwdContinue:
    case SPLSymType::KwdBreak:
    case SPLSymType::KwdReturn: {
        os << ControlSeq::Bold << ControlSeq::Magenta;
        break;
    }
    case SPLSymType::ID:
    case SPLSymType::TypedefID: {
        os << ControlSeq::Bold << ControlSeq::BrightBlue;
        break;
    }
    case SPLSymType::OpAssign:
    case SPLSymType::OpMulAssign:
    case SPLSymType::OpDivAssign:
    case SPLSymType::OpModAssign:
    case SPLSymType::OpPlusAssign:
    case SPLSymType::OpMinusAssign:
    case SPLSymType::OpLShiftAssign:
    case SPLSymType::OpRShiftAssign:
    case SPLSymType::OpBAndAssign:
    case SPLSymType::OpBXorAssign:
    case SPLSymType::OpBOrAssign:
    case SPLSymType::OpAnd:
    case SPLSymType::OpOr:
    case SPLSymType::OpNot:
    case SPLSymType::OpLT:
    case SPLSymType::OpLE:
    case SPLSymType::OpGT:
    case SPLSymType::OpGE:
    case SPLSymType::OpNE:
    case SPLSymType::OpEQ:
    case SPLSymType::OpQMark:
    case SPLSymType::OpColon:
    case SPLSymType::OpLShift:
    case SPLSymType::OpRShift:
    case SPLSymType::OpBAnd:
    case SPLSymType::OpBOr:
    case SPLSymType::OpBNot:
    case SPLSymType::OpBXor:
    case SPLSymType::OpDPlus:
    case SPLSymType::OpDMinus:
    case SPLSymType::OpPlus:
    case SPLSymType::OpMinus:
    case SPLSymType::OpAstrk:
    case SPLSymType::OpDiv:
    case SPLSymType::OpMod:
    case SPLSymType::OpDot:
    case SPLSymType::OpRArrow:
    case SPLSymType::OpSizeOf:
    case SPLSymType::OpLSB:
    case SPLSymType::OpRSB:
    case SPLSymType::OpComma:
    case SPLSymType::OpEllipsis: {
        break;
    }
    case SPLSymType::PSemi:
    case SPLSymType::PLC:
    case SPLSymType::PRC:
    case SPLSymType::PLP:
    case SPLSymType::PRP: {
        os << ControlSeq::Green;
        break;
    }
    case SPLSymType::UIntLiteral:
    case SPLSymType::SIntLiteral:
    case SPLSymType::FloatLiteral: {
        os << ControlSeq::Bold << ControlSeq::Green;
        break;
    }
    case SPLSymType::CharLiteral:
    case SPLSymType::StrUnit: {
        os << ControlSeq::Yellow;
        break;
    }
    case SPLSymType::SubscriptExpr:
    case SPLSymType::CallExpr:
    case SPLSymType::AccessExpr:
    case SPLSymType::ExplicitCastExpr:
    case SPLSymType::ImplicitCastExpr:
    case SPLSymType::AddrOfExpr:
    case SPLSymType::DerefExpr:
    case SPLSymType::SizeOfExpr: {
        os << ControlSeq::Bold << ControlSeq::BrightMagenta;
        break;
    }
    case SPLSymType::KwdThen:
    case SPLSymType::DecltrPrec:
    case SPLSymType::FuncDeclPrec:
    case SPLSymType::OpUnaryPrec:
    case SPLSymType::PLParen:
    case SPLSymType::PRParen:
    case SPLSymType::PLSBracket:
    case SPLSymType::PRSBracket:
    case SPLSymType::ParseRoot:
    case SPLSymType::TransUnit:
    case SPLSymType::ExternDeclList:
    case SPLSymType::ExternDecl:
    case SPLSymType::DeclSpec:
    case SPLSymType::StorageSpec:
    case SPLSymType::SpecQualList:
    case SPLSymType::TypeSpec:
    case SPLSymType::FuncSpec:
    case SPLSymType::TypeQual:
    case SPLSymType::TypeName:
    case SPLSymType::BuiltinTypeSpec:
    case SPLSymType::AbsDecltr:
    case SPLSymType::DirAbsDecltr:
    case SPLSymType::StructOrUnionSpec:
    case SPLSymType::StructOrUnion:
    case SPLSymType::StructDeclBody:
    case SPLSymType::StructDeclList:
    case SPLSymType::StructDecl:
    case SPLSymType::StructDecltrList:
    case SPLSymType::StructDecltr:
    case SPLSymType::EnumSpec:
    case SPLSymType::EnumBody:
    case SPLSymType::EnumeratorList:
    case SPLSymType::Enumerator:
    case SPLSymType::EnumConst:
    case SPLSymType::Decltr:
    case SPLSymType::DirDecltr:
    case SPLSymType::WrappedDirDecltr:
    // case SPLSymType::PtrDecltr:
    case SPLSymType::TypeQualList:
    case SPLSymType::Decl:
    case SPLSymType::DirDecl:
    case SPLSymType::InitDecltrList:
    case SPLSymType::InitDecltr:
    case SPLSymType::Initializer:
    case SPLSymType::InitializerList:
    case SPLSymType::Designation:
    case SPLSymType::DesignatorList:
    case SPLSymType::Designator:
    case SPLSymType::FuncDef:
    case SPLSymType::FuncProto:
    case SPLSymType::FuncDecltr:
    case SPLSymType::DirFuncDecltr:
    case SPLSymType::DirDecltrForFunc:
    case SPLSymType::ParamTypeList:
    case SPLSymType::ParamList:
    case SPLSymType::ParamDecltr: {
        os << ControlSeq::Bold << ControlSeq::Green;
        break;
    }
    case SPLSymType::CompStmt:
    case SPLSymType::GeneralStmtList:
    case SPLSymType::Stmt:
    case SPLSymType::ExprStmt:
    case SPLSymType::SelStmt:
    case SPLSymType::LabeledStmt:
    case SPLSymType::JumpStmt:
    case SPLSymType::IterStmt: {
        os << ControlSeq::Bold << ControlSeq::BrightMagenta;
        break;
    }
    case SPLSymType::ForLoopBody:
    case SPLSymType::ConstExpr:
    case SPLSymType::Constant:
    case SPLSymType::PrimaryExpr:
    case SPLSymType::PostfixExpr:
    case SPLSymType::MemberAccessOp:
    case SPLSymType::UnaryExpr:
    case SPLSymType::UnaryArithOp:
    case SPLSymType::CastExpr:
    case SPLSymType::MulExpr:
    case SPLSymType::MulOp:
    case SPLSymType::DivOp:
    case SPLSymType::AddExpr:
    case SPLSymType::AddOp:
    case SPLSymType::ShiftExpr:
    case SPLSymType::ShiftOp:
    case SPLSymType::RelExpr:
    case SPLSymType::RelOp:
    case SPLSymType::EqualityExpr:
    case SPLSymType::EqualityOp:
    case SPLSymType::OpBAndExpr:
    case SPLSymType::OpBXorExpr:
    case SPLSymType::OpBOrExpr:
    case SPLSymType::LogicalOpAndExpr:
    case SPLSymType::LogicalOpOrExpr:
    case SPLSymType::CondExpr:
    case SPLSymType::AssignExpr:
    case SPLSymType::AssignOp:
    case SPLSymType::Expr:
    case SPLSymType::InitExpr:
    case SPLSymType::ArgList: {
        os << ControlSeq::Bold << ControlSeq::Green;
        break;
    }
    case SPLSymType::StringLiteral:
    case SPLSymType::IDWrapper: {
        os << ControlSeq::Bold << ControlSeq::Yellow;
        break;
    }
    }
    return os;
}
} // namespace internal

} // namespace splc