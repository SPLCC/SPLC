#include "AST/DerivedAST.hh"

using namespace splc;

using CS = splc::utils::logging::ControlSeq;

//// YYNTOKENS-AST declaration slot

//// YYEMPTY-AST declaration slot

//// YYEOF-AST declaration slot

//// YYerror-AST declaration slot

//// YYUNDEF-AST declaration slot

//===----------------------------------------------------------------------===//
//                              Storage Qualifiers
//===----------------------------------------------------------------------===//
// KwdAuto-AST declaration slot

// KwdExtern-AST declaration slot

// KwdRegister-AST declaration slot

// KwdStatic-AST declaration slot

// KwdTypedef-AST declaration slot

//===----------------------------------------------------------------------===//
//                              Type Qualifiers
//===----------------------------------------------------------------------===//
// KwdConst-AST declaration slot

// KwdRestrict-AST declaration slot

// KwdVolatile-AST declaration slot

// KwdInline-AST declaration slot

//===----------------------------------------------------------------------===//
//                              Type Specifiers
//===----------------------------------------------------------------------===//
// VoidTy
// CharTy
// ShortTy
// IntTy
// SignedTy
// UnsignedTy
// LongTy
// FloatTy
// DoubleTy

//===----------------------------------------------------------------------===//
//                           Aggregate Type Keywords
//===----------------------------------------------------------------------===//
// KwdEnum-AST declaration slot

// KwdStruct-AST declaration slot

// KwdUnion-AST declaration slot

//===----------------------------------------------------------------------===//
//                            Control-Flow Keywords
//===----------------------------------------------------------------------===//
// KwdIf-AST declaration slot

// KwdElse-AST declaration slot

// KwdSwitch-AST declaration slot

// KwdWhile-AST declaration slot

// KwdFor-AST declaration slot

// KwdDo-AST declaration slot

// KwdDefault-AST declaration slot

// KwdCase-AST declaration slot

// KwdGoto-AST declaration slot

// KwdContinue-AST declaration slot

// KwdBreak-AST declaration slot

// KwdReturn-AST declaration slot

//===----------------------------------------------------------------------===//
//                                     IDs
//===----------------------------------------------------------------------===//
// ID-AST declaration slot

// TypedefID-AST declaration slot

//===----------------------------------------------------------------------===//
//                                   Operators
//===----------------------------------------------------------------------===//
// OpAssign-AST declaration slot

// OpMulAssign-AST declaration slot

// OpDivAssign-AST declaration slot

// OpModAssign-AST declaration slot

// OpPlusAssign-AST declaration slot

// OpMinusAssign-AST declaration slot

// OpLShiftAssign-AST declaration slot

// OpRShiftAssign-AST declaration slot

// OpBAndAssign-AST declaration slot

// OpBXorAssign-AST declaration slot

// OpBOrAssign-AST declaration slot

// OpAnd-AST declaration slot

// OpOr-AST declaration slot

// OpNot-AST declaration slot

// OpLT-AST declaration slot

// OpLE-AST declaration slot

// OpGT-AST declaration slot

// OpGE-AST declaration slot

// OpNE-AST declaration slot

// OpEQ-AST declaration slot

// OpQMark-AST declaration slot

// OpColon-AST declaration slot

// OpLShift-AST declaration slot

// OpRShift-AST declaration slot

// OpBAnd-AST declaration slot

// OpBOr-AST declaration slot

// OpBNot-AST declaration slot

// OpBXor-AST declaration slot

// OpDPlus-AST declaration slot

// OpDMinus-AST declaration slot

// OpPlus-AST declaration slot

// OpMinus-AST declaration slot

// OpAstrk-AST declaration slot

// OpDiv-AST declaration slot

// OpMod-AST declaration slot

// OpDot-AST declaration slot

// OpRArrow-AST declaration slot

// OpSizeOf-AST declaration slot

// OpLSB-AST declaration slot

// OpRSB-AST declaration slot

// OpComma-AST declaration slot

// OpEllipsis-AST declaration slot

//===----------------------------------------------------------------------===//
//                                  Punctuators
//===----------------------------------------------------------------------===//
// PSemi-AST declaration slot

// PLC-AST declaration slot

// PRC-AST declaration slot

// PLP-AST declaration slot

// PRP-AST declaration slot

//===----------------------------------------------------------------------===//
//                                   Literals
//===----------------------------------------------------------------------===//
// UIntLiteral-AST declaration slot

// SIntLiteral-AST declaration slot

// FloatLiteral-AST declaration slot

// CharLiteral-AST declaration slot

// StrUnit-AST declaration slot

//===----------------------------------------------------------------------===//
//                                 Expressions
//===----------------------------------------------------------------------===//
// SubscriptExpr-AST declaration slot

// CallExpr-AST declaration slot

// AccessExpr-AST declaration slot

// ExplicitCastExpr-AST declaration slot

// AddrOfExpr-AST declaration slot

// DerefExpr-AST declaration slot

// SizeOfExpr-AST declaration slot

//===----------------------------------------------------------------------===//
//                                 Productions
//===----------------------------------------------------------------------===//
// ParseRoot-AST declaration slot

// TransUnit-AST declaration slot

// ExternDeclList-AST declaration slot

// ExternDecl-AST declaration slot

// DeclSpec-AST declaration slot

Ptr<DeclSpecAST> AST::toDeclSpecAST() noexcept
{
    return std::dynamic_pointer_cast<DeclSpecAST>(shared_from_this());
}

// StorageSpec-AST declaration slot

// SpecQualList-AST declaration slot

// TypeSpec-AST declaration slot

// FuncSpec-AST declaration slot

// TypeQual-AST declaration slot

// TypeName-AST declaration slot

// BuiltinTypeSpec-AST declaration slot

// AbsDecltr-AST declaration slot

// DirAbsDecltr-AST declaration slot

// StructOrUnionSpec-AST declaration slot

// StructOrUnion-AST declaration slot

// StructDeclBody-AST declaration slot

// StructDeclList-AST declaration slot

// StructDecl-AST declaration slot

// StructDecltrList-AST declaration slot

// StructDecltr-AST declaration slot

// EnumSpec-AST declaration slot

// EnumBody-AST declaration slot

// EnumeratorList-AST declaration slot

// Enumerator-AST declaration slot

// EnumConst-AST declaration slot

// Decltr-AST declaration slot

// DirDecltr-AST declaration slot

// WrappedDirDecltr-AST declaration slot

// TypeQualList-AST declaration slot

// Decl-AST declaration slot

// DirDecl-AST declaration slot

// InitDecltrList-AST declaration slot

// InitDecltr-AST declaration slot

// Initializer-AST declaration slot

// InitializerList-AST declaration slot

// Designation-AST declaration slot

// DesignatorList-AST declaration slot

// Designator-AST declaration slot

// FuncDef-AST declaration slot

// FuncProto-AST declaration slot

// FuncDecltr-AST declaration slot

// DirFuncDecltr-AST declaration slot

// DirDecltrForFunc-AST declaration slot

// ParamTypeList-AST declaration slot

// ParamList-AST declaration slot

Ptr<ParamListAST> AST::toParamListAST() noexcept
{
    return std::dynamic_pointer_cast<ParamListAST>(shared_from_this());
}

// ParamDecltr-AST declaration slot

// CompStmt-AST declaration slot

// GeneralStmtList-AST declaration slot

// Stmt-AST declaration slot

// ExprStmt-AST declaration slot

// SelStmt-AST declaration slot

// LabeledStmt-AST declaration slot

// JumpStmt-AST declaration slot

// IterStmt-AST declaration slot

// ForLoopBody-AST declaration slot

// ConstExpr-AST declaration slot

// Constant-AST declaration slot

// PrimaryExpr-AST declaration slot

// PostfixExpr-AST declaration slot

// MemberAccessOp-AST declaration slot

// UnaryExpr-AST declaration slot

// UnaryArithOp-AST declaration slot

// CastExpr-AST declaration slot

// MulExpr-AST declaration slot

// MulOp-AST declaration slot

// DivOp-AST declaration slot

// AddExpr-AST declaration slot

// AddOp-AST declaration slot

// ShiftExpr-AST declaration slot

// ShiftOp-AST declaration slot

// RelExpr-AST declaration slot

// RelOp-AST declaration slot

// EqualityExpr-AST declaration slot

// EqualityOp-AST declaration slot

// OpBAndExpr-AST declaration slot

// OpBXorExpr-AST declaration slot

// OpBOrExpr-AST declaration slot

// LogicalOpAndExpr-AST declaration slot

// LogicalOpOrExpr-AST declaration slot

// CondExpr-AST declaration slot

// AssignExpr-AST declaration slot

// AssignOp-AST declaration slot

// Expr-AST declaration slot

// InitExpr-AST declaration slot

// ArgList-AST declaration slot

// StringLiteral-AST declaration slot

// IDWrapper-AST declaration slot