#ifndef __SPLC_AST_ASTSYMBOL_HH__
#define __SPLC_AST_ASTSYMBOL_HH__ 1

#include "Core/Utils.hh"
#include <iostream>

namespace splc {

namespace internal {

/// Automatically generated by GNU Bison

enum symbol_kind_type {
    YYNTOKENS = 104, ///< Number of tokens.
    YYEMPTY = -2,
    YYEOF = 0,               // "end of file"
    YYerror = 1,             // error
    YYUNDEF = 2,             // "invalid token"
    KwdAuto = 3,             // KwdAuto
    KwdExtern = 4,           // KwdExtern
    KwdRegister = 5,         // KwdRegister
    KwdStatic = 6,           // KwdStatic
    KwdTypedef = 7,          // KwdTypedef
    KwdConst = 8,            // KwdConst
    KwdRestrict = 9,         // KwdRestrict
    KwdVolatile = 10,        // KwdVolatile
    KwdInline = 11,          // KwdInline
    VoidTy = 12,             // VoidTy
    IntTy = 13,              // IntTy
    SignedTy = 14,           // SignedTy
    UnsignedTy = 15,         // UnsignedTy
    LongTy = 16,             // LongTy
    FloatTy = 17,            // FloatTy
    DoubleTy = 18,           // DoubleTy
    CharTy = 19,             // CharTy
    KwdEnum = 20,            // KwdEnum
    KwdStruct = 21,          // KwdStruct
    KwdUnion = 22,           // KwdUnion
    KwdIf = 23,              // KwdIf
    KwdElse = 24,            // KwdElse
    KwdSwitch = 25,          // KwdSwitch
    KwdWhile = 26,           // KwdWhile
    KwdFor = 27,             // KwdFor
    KwdDo = 28,              // KwdDo
    KwdDefault = 29,         // KwdDefault
    KwdCase = 30,            // KwdCase
    KwdGoto = 31,            // KwdGoto
    KwdContinue = 32,        // KwdContinue
    KwdBreak = 33,           // KwdBreak
    KwdReturn = 34,          // KwdReturn
    ID = 35,                 // ID
    TypedefID = 36,          // TypedefID
    OpAssign = 37,           // OpAssign
    OpMulAssign = 38,        // OpMulAssign
    OpDivAssign = 39,        // OpDivAssign
    OpModAssign = 40,        // OpModAssign
    OpPlusAssign = 41,       // OpPlusAssign
    OpMinusAssign = 42,      // OpMinusAssign
    OpLShiftAssign = 43,     // OpLShiftAssign
    OpRShiftAssign = 44,     // OpRShiftAssign
    OpBAndAssign = 45,       // OpBAndAssign
    OpBXorAssign = 46,       // OpBXorAssign
    OpBOrAssign = 47,        // OpBOrAssign
    OpAnd = 48,              // OpAnd
    OpOr = 49,               // OpOr
    OpNot = 50,              // OpNot
    OpLT = 51,               // OpLT
    OpLE = 52,               // OpLE
    OpGT = 53,               // OpGT
    OpGE = 54,               // OpGE
    OpNE = 55,               // OpNE
    OpEQ = 56,               // OpEQ
    OpQMark = 57,            // OpQMark
    OpColon = 58,            // OpColon
    OpLShift = 59,           // OpLShift
    OpRShift = 60,           // OpRShift
    OpBAnd = 61,             // OpBAnd
    OpBOr = 62,              // OpBOr
    OpBNot = 63,             // OpBNot
    OpBXor = 64,             // OpBXor
    OpDPlus = 65,            // OpDPlus
    OpDMinus = 66,           // OpDMinus
    OpPlus = 67,             // OpPlus
    OpMinus = 68,            // OpMinus
    OpAstrk = 69,            // OpAstrk
    OpDiv = 70,              // OpDiv
    OpMod = 71,              // OpMod
    OpDot = 72,              // OpDot
    OpRArrow = 73,           // OpRArrow
    OpSizeOf = 74,           // OpSizeOf
    OpLSB = 75,              // OpLSB
    OpRSB = 76,              // OpRSB
    OpComma = 77,            // OpComma
    OpEllipsis = 78,         // OpEllipsis
    PSemi = 79,              // PSemi
    PLC = 80,                // PLC
    PRC = 81,                // PRC
    PLP = 82,                // PLP
    PRP = 83,                // PRP
    UIntLiteral = 84,        // UIntLiteral
    SIntLiteral = 85,        // SIntLiteral
    FloatLiteral = 86,       // FloatLiteral
    CharLiteral = 87,        // CharLiteral
    StrUnit = 88,            // StrUnit
    SubscriptExpr = 89,      // SubscriptExpr
    CallExpr = 90,           // CallExpr
    AccessExpr = 91,         // AccessExpr
    ExplicitCastExpr = 92,   // ExplicitCastExpr
    AddrOfExpr = 93,         // AddrOfExpr
    DerefExpr = 94,          // DerefExpr
    SizeOfExpr = 95,         // SizeOfExpr
    KwdThen = 96,            // KwdThen
    DecltrPrec = 97,         // DecltrPrec
    FuncDeclPrec = 98,       // FuncDeclPrec
    OpUnaryPrec = 99,        // OpUnaryPrec
    PLParen = 100,           // PLParen
    PRParen = 101,           // PRParen
    PLSBracket = 102,        // PLSBracket
    PRSBracket = 103,        // PRSBracket
    YYACCEPT = 104,          // $accept
    ParseRoot = 105,         // ParseRoot
    // 106_1 = 106,             // $@1
    TransUnit = 107,         // TransUnit
    ExternDeclList = 108,    // ExternDeclList
    ExternDecl = 109,        // ExternDecl
    DeclSpec = 110,          // DeclSpec
    StorageSpec = 111,       // StorageSpec
    SpecQualList = 112,      // SpecQualList
    TypeSpec = 113,          // TypeSpec
    FuncSpec = 114,          // FuncSpec
    TypeQual = 115,          // TypeQual
    TypeName = 116,          // TypeName
    BuiltinTypeSpec = 117,   // BuiltinTypeSpec
    AbsDecltr = 118,         // AbsDecltr
    DirAbsDecltr = 119,      // DirAbsDecltr
    StructOrUnionSpec = 120, // StructOrUnionSpec
    StructOrUnion = 121,     // StructOrUnion
    StructDeclBody = 122,    // StructDeclBody
    StructDeclList = 123,    // StructDeclList
    StructDecl = 124,        // StructDecl
    StructDecltrList = 125,  // StructDecltrList
    StructDecltr = 126,      // StructDecltr
    EnumSpec = 127,          // EnumSpec
    EnumBody = 128,          // EnumBody
    EnumeratorList = 129,    // EnumeratorList
    Enumerator = 130,        // Enumerator
    EnumConst = 131,         // EnumConst
    Decltr = 132,            // Decltr
    DirDecltr = 133,         // DirDecltr
    WrappedDirDecltr = 134,  // WrappedDirDecltr
    PtrDecl = 135,           // PtrDecl
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
    FuncDecltr = 147,        // FuncDecltr
    DirFuncDecltr = 148,     // DirFuncDecltr
    DirDecltrForFunc = 149,  // DirDecltrForFunc
    ParamTypeList = 150,     // ParamTypeList
    ParamList = 151,         // ParamList
    ParamDecl = 152,         // ParamDecl
    CompStmt = 153,          // CompStmt
    GeneralStmtList = 154,   // GeneralStmtList
    Stmt = 155,              // Stmt
    ExprStmt = 156,          // ExprStmt
    SelStmt = 157,           // SelStmt
    LabeledStmt = 158,       // LabeledStmt
    JumpStmt = 159,          // JumpStmt
    IterStmt = 160,          // IterStmt
    ForLoopBody = 161,       // ForLoopBody
    ConstExpr = 162,         // ConstExpr
    Constant = 163,          // Constant
    PrimaryExpr = 164,       // PrimaryExpr
    PostfixExpr = 165,       // PostfixExpr
    MemberAcessOp = 166,     // MemberAcessOp
    UnaryExpr = 167,         // UnaryExpr
    UnaryArithOp = 168,      // UnaryArithOp
    CastExpr = 169,          // CastExpr
    MulExpr = 170,           // MulExpr
    MulOp = 171,             // MulOp
    DivOp = 172,             // DivOp
    AddExpr = 173,           // AddExpr
    AddOp = 174,             // AddOp
    ShiftExpr = 175,         // ShiftExpr
    ShiftOp = 176,           // ShiftOp
    RelExpr = 177,           // RelExpr
    RelOp = 178,             // RelOp
    EqualityExpr = 179,      // EqualityExpr
    EqualityOp = 180,        // EqualityOp
    OpBAndExpr = 181,        // OpBAndExpr
    OpBXorExpr = 182,        // OpBXorExpr
    OpBOrExpr = 183,         // OpBOrExpr
    LogicalOpAndExpr = 184,  // LogicalOpAndExpr
    LogicalOpOrExpr = 185,   // LogicalOpOrExpr
    CondExpr = 186,          // CondExpr
    AssignExpr = 187,        // AssignExpr
    AssignOp = 188,          // AssignOp
    Expr = 189,              // Expr
    InitExpr = 190,          // InitExpr
    ArgList = 191,           // ArgList
    StringLiteral = 192,     // StringLiteral
    IDWrapper = 193          // IDWrapper
};

using SPLSymbolType = symbol_kind_type;

std::string_view getSymbolName(SPLSymbolType sym) noexcept;

std::ostream &printSymbolConsoleTraits(std::ostream &os,
                                       SPLSymbolType symbol) noexcept;

std::ostream &operator<<(std::ostream &os, SPLSymbolType sym) noexcept;

} // namespace internal

//===----------------------------------------------------------------------===//
//                        ASTSymbolType Encapsulations
//===----------------------------------------------------------------------===//

/// This is used to separate code in `Parser.yy` from directly
/// using its symbol_kind_type, thus creating encapsulation and
/// allow easy debugging/replacement of the actual ASTSymbolType.
// using ASTSymbolType = IO::Parser::symbol_kind::symbol_kind_type;
using ASTSymbolType = internal::SPLSymbolType;

inline std::string_view getASTSymbolName(ASTSymbolType sym) noexcept
{
    // return IO::Parser::symbol_name(sym);
    return internal::getSymbolName(sym);
}

// TODO: maybe implement
bool isSymExpr(ASTSymbolType sym) noexcept;
bool isSymStmt(ASTSymbolType sym) noexcept;
bool isSymTypeSpecifier(ASTSymbolType sym) noexcept;
bool isSymTypeQualifier(ASTSymbolType sym) noexcept;
bool isSymStruct(ASTSymbolType sym) noexcept;
bool isSymStruct(ASTSymbolType sym) noexcept;

inline std::ostream &operator<<(std::ostream &os, ASTSymbolType sym) noexcept
{
    return internal::operator<<(os, sym);
}

//===----------------------------------------------------------------------===//
//                      ASTSymbolType Helper Templates
//===----------------------------------------------------------------------===//
template <class... Types>
concept AllAreASTSymbolType =
    (std::is_same_v<ASTSymbolType,
                    typename std::remove_reference<Types>::type> &&
     ...);

template <AllAreASTSymbolType... SourceTypes>
bool isASTSymbolTypeOneOfThem(ASTSymbolType type, SourceTypes &&...otherTypes)
{
    return ((type == otherTypes) || ...);
}

class ASTSymbolColorManipulator {
  public:
    friend ASTSymbolColorManipulator getASTSymbolColor(ASTSymbolType sym);
    friend std::ostream &operator<<(std::ostream &os,
                                    const ASTSymbolColorManipulator &m);

  private:
    ASTSymbolType sym;
    ASTSymbolColorManipulator(ASTSymbolType sym_) : sym{sym_} {}
};

inline ASTSymbolColorManipulator getASTSymbolColor(ASTSymbolType sym)
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