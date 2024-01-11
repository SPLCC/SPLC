#ifndef __SPLC_AST_ASTSYMBOL_HH__
#define __SPLC_AST_ASTSYMBOL_HH__ 1

#include "Core/Utils.hh"
#include <iostream>

namespace splc {

namespace internal {

// TODO: refactor to intergrate the methods into a single class (instead of
//       multiple helper methods outside)

/// Automatically generated by GNU Bison

enum class symbol_kind_type {
    YYNTOKENS = 105, ///< Number of tokens.
    YYEMPTY = -2,
    YYEOF = 0,             // "end of file"
    YYerror = 1,           // error
    YYUNDEF = 2,           // "invalid token"
    KwdAuto = 3,           // KwdAuto
    KwdExtern = 4,         // KwdExtern
    KwdRegister = 5,       // KwdRegister
    KwdStatic = 6,         // KwdStatic
    KwdTypedef = 7,        // KwdTypedef
    KwdConst = 8,          // KwdConst
    KwdRestrict = 9,       // KwdRestrict
    KwdVolatile = 10,      // KwdVolatile
    KwdInline = 11,        // KwdInline
    VoidTy = 12,           // VoidTy
    CharTy = 13,           // CharTy
    ShortTy = 14,          // ShortTy
    IntTy = 15,            // IntTy
    SignedTy = 16,         // SignedTy
    UnsignedTy = 17,       // UnsignedTy
    LongTy = 18,           // LongTy
    FloatTy = 19,          // FloatTy
    DoubleTy = 20,         // DoubleTy
    KwdEnum = 21,          // KwdEnum
    KwdStruct = 22,        // KwdStruct
    KwdUnion = 23,         // KwdUnion
    KwdIf = 24,            // KwdIf
    KwdElse = 25,          // KwdElse
    KwdSwitch = 26,        // KwdSwitch
    KwdWhile = 27,         // KwdWhile
    KwdFor = 28,           // KwdFor
    KwdDo = 29,            // KwdDo
    KwdDefault = 30,       // KwdDefault
    KwdCase = 31,          // KwdCase
    KwdGoto = 32,          // KwdGoto
    KwdContinue = 33,      // KwdContinue
    KwdBreak = 34,         // KwdBreak
    KwdReturn = 35,        // KwdReturn
    ID = 36,               // ID
    TypedefID = 37,        // TypedefID
    OpAssign = 38,         // OpAssign
    OpMulAssign = 39,      // OpMulAssign
    OpDivAssign = 40,      // OpDivAssign
    OpModAssign = 41,      // OpModAssign
    OpPlusAssign = 42,     // OpPlusAssign
    OpMinusAssign = 43,    // OpMinusAssign
    OpLShiftAssign = 44,   // OpLShiftAssign
    OpRShiftAssign = 45,   // OpRShiftAssign
    OpBAndAssign = 46,     // OpBAndAssign
    OpBXorAssign = 47,     // OpBXorAssign
    OpBOrAssign = 48,      // OpBOrAssign
    OpAnd = 49,            // OpAnd
    OpOr = 50,             // OpOr
    OpNot = 51,            // OpNot
    OpLT = 52,             // OpLT
    OpLE = 53,             // OpLE
    OpGT = 54,             // OpGT
    OpGE = 55,             // OpGE
    OpNE = 56,             // OpNE
    OpEQ = 57,             // OpEQ
    OpQMark = 58,          // OpQMark
    OpColon = 59,          // OpColon
    OpLShift = 60,         // OpLShift
    OpRShift = 61,         // OpRShift
    OpBAnd = 62,           // OpBAnd
    OpBOr = 63,            // OpBOr
    OpBNot = 64,           // OpBNot
    OpBXor = 65,           // OpBXor
    OpDPlus = 66,          // OpDPlus
    OpDMinus = 67,         // OpDMinus
    OpPlus = 68,           // OpPlus
    OpMinus = 69,          // OpMinus
    OpAstrk = 70,          // OpAstrk
    OpDiv = 71,            // OpDiv
    OpMod = 72,            // OpMod
    OpDot = 73,            // OpDot
    OpRArrow = 74,         // OpRArrow
    OpSizeOf = 75,         // OpSizeOf
    OpLSB = 76,            // OpLSB
    OpRSB = 77,            // OpRSB
    OpComma = 78,          // OpComma
    OpEllipsis = 79,       // OpEllipsis
    PSemi = 80,            // PSemi
    PLC = 81,              // PLC
    PRC = 82,              // PRC
    PLP = 83,              // PLP
    PRP = 84,              // PRP
    UIntLiteral = 85,      // UIntLiteral
    SIntLiteral = 86,      // SIntLiteral
    FloatLiteral = 87,     // FloatLiteral
    CharLiteral = 88,      // CharLiteral
    StrUnit = 89,          // StrUnit
    SubscriptExpr = 90,    // SubscriptExpr
    CallExpr = 91,         // CallExpr
    AccessExpr = 92,       // AccessExpr
    ExplicitCastExpr = 93, // ExplicitCastExpr
    AddrOfExpr = 94,       // AddrOfExpr
    DerefExpr = 95,        // DerefExpr
    SizeOfExpr = 96,       // SizeOfExpr
    KwdThen = 97,          // KwdThen
    DecltrPrec = 98,       // DecltrPrec
    FuncDeclPrec = 99,     // FuncDeclPrec
    OpUnaryPrec = 100,     // OpUnaryPrec
    PLParen = 101,         // PLParen
    PRParen = 102,         // PRParen
    PLSBracket = 103,      // PLSBracket
    PRSBracket = 104,      // PRSBracket
    YYACCEPT = 105,        // $accept
    ParseRoot = 106,       // ParseRoot
    // 107_1 = 107,                           // $@1
    TransUnit = 108,         // TransUnit
    ExternDeclList = 109,    // ExternDeclList
    ExternDecl = 110,        // ExternDecl
    DeclSpec = 111,          // DeclSpec
    StorageSpec = 112,       // StorageSpec
    SpecQualList = 113,      // SpecQualList
    TypeSpec = 114,          // TypeSpec
    FuncSpec = 115,          // FuncSpec
    TypeQual = 116,          // TypeQual
    TypeName = 117,          // TypeName
    BuiltinTypeSpec = 118,   // BuiltinTypeSpec
    AbsDecltr = 119,         // AbsDecltr
    DirAbsDecltr = 120,      // DirAbsDecltr
    StructOrUnionSpec = 121, // StructOrUnionSpec
    StructOrUnion = 122,     // StructOrUnion
    StructDeclBody = 123,    // StructDeclBody
    StructDeclList = 124,    // StructDeclList
    StructDecl = 125,        // StructDecl
    StructDecltrList = 126,  // StructDecltrList
    StructDecltr = 127,      // StructDecltr
    EnumSpec = 128,          // EnumSpec
    EnumBody = 129,          // EnumBody
    EnumeratorList = 130,    // EnumeratorList
    Enumerator = 131,        // Enumerator
    EnumConst = 132,         // EnumConst
    Decltr = 133,            // Decltr
    DirDecltr = 134,         // DirDecltr
    WrappedDirDecltr = 135,  // WrappedDirDecltr
    TypeQualList = 136,      // TypeQualList
    Decl = 137,              // Decl
    DirDecl = 138,           // DirDecl
    InitDecltrList = 139,    // InitDecltrList
    InitDecltr = 140,        // InitDecltr
    Initializer = 141,       // Initializer
    InitializerList = 142,   // InitializerList
    Designation = 143,       // Designation
    DesignatorList = 144,    // DesignatorList
    Designator = 145,        // Designator
    FuncDef = 146,           // FuncDef
    FuncProto = 147,          // FuncProto
    FuncDecltr = 148,        // FuncDecltr
    DirFuncDecltr = 149,     // DirFuncDecltr
    DirDecltrForFunc = 150,  // DirDecltrForFunc
    ParamTypeList = 151,     // ParamTypeList
    ParamList = 152,         // ParamList
    ParamDecltr = 153,       // ParamDecltr
    CompStmt = 154,          // CompStmt
    GeneralStmtList = 155,   // GeneralStmtList
    Stmt = 156,              // Stmt
    ExprStmt = 157,          // ExprStmt
    SelStmt = 158,           // SelStmt
    LabeledStmt = 159,       // LabeledStmt
    JumpStmt = 160,          // JumpStmt
    IterStmt = 161,          // IterStmt
    ForLoopBody = 162,       // ForLoopBody
    ConstExpr = 163,         // ConstExpr
    Constant = 164,          // Constant
    PrimaryExpr = 165,       // PrimaryExpr
    PostfixExpr = 166,       // PostfixExpr
    MemberAccessOp = 167,    // MemberAccessOp
    UnaryExpr = 168,         // UnaryExpr
    UnaryArithOp = 169,      // UnaryArithOp
    CastExpr = 170,          // CastExpr
    MulExpr = 171,           // MulExpr
    MulOp = 172,             // MulOp
    DivOp = 173,             // DivOp
    AddExpr = 174,           // AddExpr
    AddOp = 175,             // AddOp
    ShiftExpr = 176,         // ShiftExpr
    ShiftOp = 177,           // ShiftOp
    RelExpr = 178,           // RelExpr
    RelOp = 179,             // RelOp
    EqualityExpr = 180,      // EqualityExpr
    EqualityOp = 181,        // EqualityOp
    OpBAndExpr = 182,        // OpBAndExpr
    OpBXorExpr = 183,        // OpBXorExpr
    OpBOrExpr = 184,         // OpBOrExpr
    LogicalOpAndExpr = 185,  // LogicalOpAndExpr
    LogicalOpOrExpr = 186,   // LogicalOpOrExpr
    CondExpr = 187,          // CondExpr
    AssignExpr = 188,        // AssignExpr
    AssignOp = 189,          // AssignOp
    Expr = 190,              // Expr
    InitExpr = 191,          // InitExpr
    ArgList = 192,           // ArgList
    StringLiteral = 193,     // StringLiteral
    IDWrapper = 194          // IDWrapper
};

using SPLSymType = symbol_kind_type;

std::string_view getSymbolName(SPLSymType sym) noexcept;

std::ostream &printSymbolConsoleTraits(std::ostream &os,
                                       SPLSymType symbol) noexcept;

inline std::ostream &operator<<(std::ostream &os, symbol_kind_type sym) noexcept
{
    return os << internal::getSymbolName(sym);
}

} // namespace internal

//===----------------------------------------------------------------------===//
//                        ASTSymbolType Encapsulations
//===----------------------------------------------------------------------===//

/// This is used to separate code in `Parser.yy` from directly
/// using its symbol_kind_type, thus creating encapsulation and
/// allow easy debugging/replacement of the actual ASTSymbolType.
// using ASTSymbolType = IO::Parser::symbol_kind::symbol_kind_type;
using ASTSymType = internal::SPLSymType;

inline std::string_view getASTSymbolName(ASTSymType sym) noexcept
{
    // return IO::Parser::symbol_name(sym);
    return internal::getSymbolName(sym);
}

// TODO: maybe implement
bool isSymExpr(ASTSymType sym) noexcept;
bool isSymStmt(ASTSymType sym) noexcept;
bool isSymTypeSpecifier(ASTSymType sym) noexcept;
bool isSymTypeQualifier(ASTSymType sym) noexcept;
bool isSymDeclSpecChildren(ASTSymType sym) noexcept;
bool isSymStruct(ASTSymType sym) noexcept;
bool isSymStruct(ASTSymType sym) noexcept;

//===----------------------------------------------------------------------===//
//                      ASTSymbolType Helper Templates
//===----------------------------------------------------------------------===//
template <class... Types>
concept AllAreASTSymbolType =
    (std::is_same_v<ASTSymType, typename std::remove_cvref_t<Types>> &&
     ...);

template <AllAreASTSymbolType... SourceTypes>
bool isASTSymbolTypeOneOf(ASTSymType type, SourceTypes &&...otherTypes)
{
    return ((type == otherTypes) || ...);
}

class ASTSymbolColorManipulator {
  public:
    friend ASTSymbolColorManipulator getASTSymbolColor(ASTSymType sym);
    friend std::ostream &operator<<(std::ostream &os,
                                    const ASTSymbolColorManipulator &m);

  private:
    ASTSymType sym;
    ASTSymbolColorManipulator(ASTSymType sym_) : sym{sym_} {}
};

inline ASTSymbolColorManipulator getASTSymbolColor(ASTSymType sym)
{
    return ASTSymbolColorManipulator{sym};
}

inline std::ostream &operator<<(std::ostream &os,
                                const ASTSymbolColorManipulator &m)
{
    // using utils::logging::ControlSeq;
    // os << ControlSeq::Bold << ControlSeq::Green;
    internal::printSymbolConsoleTraits(os, m.sym);
    return os;
}

} // namespace splc

#endif // __SPLC_AST_ASTSYMBOL_HH__