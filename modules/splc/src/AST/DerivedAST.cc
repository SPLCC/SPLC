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

Type *DeclSpecAST::builtInComputeLangType() noexcept
{
    if (tyContext == nullptr) {
        splc_error() << "no type context has been provided";
    }

    Type *ret{nullptr};

    int nTypedef{0};
    int nChar{0};
    int nShort{0};
    int nInt{0};
    int nSigned{0};
    int nUnsigned{0};
    int nLong{0};
    int nFloat{0};
    int nDouble{0};

    int nAggr{0};

    for (auto &ent : getChildren()) {
        if (!ent->isTypeSpec())
            continue;

        auto realSpec = ent->getChildren()[0];

        switch (realSpec->getSymType()) {
        case ASTSymType::TypedefID: {
            if (nTypedef > 0) {
                SPLC_LOG_ERROR(&realSpec->getLocation(), false)
                    << "multiple typedef";
                return &getTyContext()->SInt32Ty;
            }
            ++nTypedef;
            break;
        }
        case ASTSymType::CharTy:
            if (nChar > 0 || nShort > 0 || nInt > 0 || nLong > 0 ||
                nFloat > 0 || nDouble > 0) {
                SPLC_LOG_ERROR(&realSpec->getLocation(), false)
                    << "multiple base type specifier";
                return &getTyContext()->SInt32Ty;
            }
            ++nChar;
            break;
        case ASTSymType::ShortTy:
            if (nChar > 0 || nShort > 0 || nInt > 0 || nLong > 0 ||
                nFloat > 0 || nDouble > 0) {
                SPLC_LOG_ERROR(&realSpec->getLocation(), false)
                    << "multiple base type specifier";
                return &getTyContext()->SInt32Ty;
            }
            ++nShort;
            break;
        case ASTSymType::IntTy:
            if (nChar > 0 || nShort > 0 || nInt > 0 || nLong > 0 ||
                nFloat > 0 || nDouble > 0) {
                SPLC_LOG_ERROR(&realSpec->getLocation(), false)
                    << "multiple base type specifier";
                return &getTyContext()->SInt32Ty;
            }
            ++nInt;
            break;
        case ASTSymType::SignedTy:
            if (nUnsigned > 0) {
                SPLC_LOG_ERROR(&realSpec->getLocation(), false)
                    << "contradictory type specifier";
                return &getTyContext()->SInt32Ty;
            }
            else if (nFloat > 0 || nDouble > 0) {
                SPLC_LOG_ERROR(&realSpec->getLocation(), false)
                    << "type specifier cannot be applied to floating-point "
                       "type";
                return &getTyContext()->SInt32Ty;
            }
            ++nSigned;
            break;
        case ASTSymType::UnsignedTy:
            if (nSigned > 0) {
                SPLC_LOG_ERROR(&realSpec->getLocation(), false)
                    << "contradictory type specifier";
                return &getTyContext()->SInt32Ty;
            }
            else if (nFloat > 0 || nDouble > 0) {
                SPLC_LOG_ERROR(&realSpec->getLocation(), false)
                    << "type specifier cannot be applied to floating-point "
                       "type";
                return &getTyContext()->SInt32Ty;
            }
            ++nUnsigned;
            break;
        case ASTSymType::LongTy:
            if (nChar > 0 || nShort > 0 || nInt > 0 || nFloat > 0 ||
                nDouble > 0) {
                SPLC_LOG_ERROR(&realSpec->getLocation(), false)
                    << "multiple base type specifier";
                return &getTyContext()->SInt32Ty;
            }
            else if (nLong == 2) {
                SPLC_LOG_ERROR(&realSpec->getLocation(), false)
                    << "too many `" << CS::Bold << "long" << CS::Reset
                    << "' as type specifier";
                return &getTyContext()->SInt32Ty;
            }
            ++nLong;
            break;
        case ASTSymType::FloatTy:
            if (nChar > 0 || nShort > 0 || nInt > 0 || nLong > 0 ||
                nFloat > 0 || nDouble > 0) {
                SPLC_LOG_ERROR(&realSpec->getLocation(), false)
                    << "multiple base type specifier";
                return &getTyContext()->SInt32Ty;
            }
            else if (nSigned > 0 || nUnsigned > 0) {
                SPLC_LOG_ERROR(&realSpec->getLocation(), false)
                    << "cannot apply to floating-point type";
                return &getTyContext()->SInt32Ty;
            }
            ++nFloat;
            break;
        case ASTSymType::DoubleTy:
            if (nChar > 0 || nShort > 0 || nInt > 0 || nLong > 0 ||
                nFloat > 0 || nDouble > 0) {
                SPLC_LOG_ERROR(&realSpec->getLocation(), false)
                    << "multiple base type specifier";
                return &getTyContext()->SInt32Ty;
            }
            else if (nSigned > 0 || nUnsigned > 0) {
                SPLC_LOG_ERROR(&realSpec->getLocation(), false)
                    << "cannot apply to floating-point type";
                return &getTyContext()->SInt32Ty;
            }
            ++nDouble;
            break;
        case ASTSymType::StructOrUnionSpec: {
            // TODO: handle struct/union
            if (ret == nullptr) {
                splc_error()
                    << "using struct/union as type specifier is unsupported";
            }
            else if (nChar > 0 || nShort > 0 || nInt > 0 || nSigned > 0 ||
                     nUnsigned > 0 || nLong > 0 || nFloat > 0 || nDouble > 0) {
                SPLC_LOG_ERROR(&realSpec->getLocation(), false)
                    << "invalid extra type specifier";
            }
            else {
                splc_error()
                    << "multiple struct/union specifier is not allowed";
            }
            break;
        }
        case ASTSymType::EnumSpec: {
            // TODO: handle enum
            if (ret == nullptr) {
                splc_error() << "using enum as type specifier is unsupported";
            }
            else if (nChar > 0 || nShort > 0 || nInt > 0 || nSigned > 0 ||
                     nUnsigned > 0 || nLong > 0 || nFloat > 0 || nDouble > 0) {
                SPLC_LOG_ERROR(&realSpec->getLocation(), false)
                    << "invalid extra type specifier";
            }
            else {
                splc_error() << "multiple enum specifier is not allowed";
            }
            break;
        }
        default:
            SPLC_LOG_ERROR(&realSpec->getLocation(), false)
                << "invalid specifier: " << realSpec->getSymType();
        }
    }

    // Process primitive type declarations
    if (nChar > 0 || nShort > 0 || nInt > 0 || nSigned > 0 || nUnsigned > 0 ||
        nLong > 0 || nFloat > 0 || nDouble > 0) {

        if (nChar > 0)
            ret = &getTyContext()->SInt8Ty;
        if (nShort > 0)
            ret = &getTyContext()->SInt16Ty;
        if (nInt > 0)
            ret = &getTyContext()->SInt32Ty;
        if (nLong == 1)
            ret = &getTyContext()->SInt32Ty;
        if (nLong == 2)
            ret = &getTyContext()->SInt64Ty;
        if (nFloat == 2)
            ret = &getTyContext()->FloatTy;
        if (nDouble == 2)
            ret = &getTyContext()->DoubleTy;

        if (nSigned > 0) {
            if (ret == nullptr)
                ret = &getTyContext()->SInt32Ty;
            else
                ret = ret->getSigned();
        }
        if (nUnsigned > 0) {
            if (ret == nullptr)
                ret = &getTyContext()->UInt32Ty;
            else
                ret = ret->getUnsigned();
        }
    }

    if (nTypedef > 0) {
        setTypedef(true);
    }

    return ret;
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
Type *DecltrAST::builtInComputeLangType() noexcept
{
    auto &children = getChildren();

    if (children[0]->isDirDecltr()) {
        return children[0]->getLangType();
    }
    else if (children[0]->isOpAstrk()) {
        auto t = children[1]->getLangType()->getPointerTo();
        return static_cast<Type *>(t);
    }
    return nullptr;
}

// DirDecltr-AST declaration slot
Type *DirDecltrAST::builtInComputeLangType() noexcept
{
    if (baseType == nullptr)
        return nullptr;

    auto &children = getChildren();

    if (children.size() == 1) {
        if (children[0]->isID()) {
            return baseType;
        }
        else if (children[0]->isWrappedDirDecltr()) {
            auto wrapped = children[0];
            return wrapped->getLangType();
        }
        return nullptr;
    }

    if (children.size() == 3) {
        // TODO(future): VLA
        SPLC_LOG_ERROR(&getLocation(), true)
            << "must specify the size of array.";
        return nullptr;
    }

    if (children.size() == 4) {
        // TODO(future): maybe support evaluating constant expressions
        // must be constant expression and only a single constant
        if (children[1]->isOpLSB()) {
            // array
            auto constNode = children[2]->getChildren()[0];
            if (!constNode->isConstant() ||
                !constNode->getChildren()[0]->isUIntLiteral()) {
                SPLC_LOG_ERROR(&constNode->getLocation(), true)
                    << "only unsigned integer is allowed";
            }
            auto arraySize =
                constNode->getChildren()[0]->getConstVal<ASTUIntType>();

            return ArrayType::get(children[0]->getLangType(), arraySize);
        }
        else if (children[1]->isParamList()) {
            // function
            std::vector<Type *> &paramTys = children[1]->getContainedTys();
            return FunctionType::get(children[0]->getLangType(), paramTys,
                                     false);
        }
        return nullptr;
    }

    return nullptr;
}

// WrappedDirDecltr-AST declaration slot

Type *WrappedDirDecltrAST::builtInComputeLangType() noexcept
{
    return getChildren()[0]->getLangType();
}

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

// FuncDecl-AST declaration slot

// FuncDecltr-AST declaration slot

// DirFuncDecltr-AST declaration slot

// DirDecltrForFunc-AST declaration slot

// ParamTypeList-AST declaration slot

// ParamList-AST declaration slot

void ParamListAST::addChild(PtrAST child) noexcept 
{
    AST::addChild(child);
    getContainedTys().push_back(child->getLangType());
}

// ParamDecltr-AST declaration slot

Type *ParamDecltrAST::builtInComputeLangType() noexcept
{
    if (getChildrenNum() == 1) {
        return getChildren()[0]->getLangType();
    }
    else if (getChildrenNum() == 2) {
        return getChildren()[1]->getLangType();
    }
    return nullptr;
}

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