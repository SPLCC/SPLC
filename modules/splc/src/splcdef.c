#include "splcdef.h"
#include "lut.h"
#include "utils.h"

int err_count = 0;
int warn_count = 0;

char *splc_loc2str(splc_loc location)
{
    char *buffer = NULL;
    if (SPLC_IS_LOC_INVALID(location))
    {
        buffer = strdup("{invalid loc}");
    }
    else
    {
        const char *locstr = "fid:%d, line:%d:%d, line:%d:%d";
        size_t needed = 1 + snprintf(NULL, 0, locstr, location.fid, location.linebegin, location.colbegin,
                                     location.lineend, location.colend);
        buffer = (char *)malloc(needed * sizeof(char));
        SPLC_ALLOC_PTR_CHECK(buffer, "cannot allocate memory for location printing)");
        sprintf(buffer, locstr, location.fid, location.linebegin, location.colbegin, location.lineend, location.colend);
    }

    return buffer;
}

// splc_loc splc_concat_loc(splc_loc l, splc_loc r)
// {
//     // TODO: determine location concatenation
// }

const char *splc_get_token_color_code(splc_token_t type)
{
    switch (type)
    {
    /* Nonterminals */
    case SPLT_NULL:
        return "\033[34m";

    case SPLT_TRANS_UNIT:
        return "\033[38;5;51m";

    /* Nonterminals: Macro Expressions */
    case SPLT_MACRO_MNTPT:
        return "\033[38;5;141m";

    case SPLT_FUNC_DEF:
    case SPLT_FUNC_DEC:
    case SPLT_DIR_FUNC_DEC:
        return "\033[38;5;229m";

    case SPLT_DECLTN_LIST:
    case SPLT_ENUM_LIST:
    case SPLT_EXT_DECLTN_LIST:
    case SPLT_EXT_DECLTN:
    case SPLT_STRUCT_DECLTN_LIST:
    case SPLT_STRUCT_DEC_LIST:
    case SPLT_INIT_DEC_LIST:
    case SPLT_STRUCT_DECLTN:
    case SPLT_STRUCT_DEC:
    case SPLT_INIT_DEC:
        return "\033[32m";

    /* Nonterminals: statements */
    case SPLT_COMP_STMT:
    case SPLT_STMT:
    case SPLT_GEN_STMT_LIST:
    // case SPLT_STMT_LIST:
    case SPLT_EXPR_STMT:
    case SPLT_SEL_STMT:
    case SPLT_ITER_STMT:
    case SPLT_FOR_LOOP_BODY:
    case SPLT_LABELED_STMT:
    case SPLT_JUMP_STMT:

    case SPLT_EXPR:
    case SPLT_PRIM_EXPR:
    case SPLT_POSTF_EXPR:
    case SPLT_UNARY_EXPR:
    case SPLT_CAST_EXPR:
    case SPLT_MUL_EXPR:
    case SPLT_ADD_EXPR:
    case SPLT_SHIFT_EXPR:
    case SPLT_REL_EXPR:
    case SPLT_EQ_EXPR:
    case SPLT_BW_AND_EXPR:
    case SPLT_BW_XOR_EXPR:
    case SPLT_BW_OR_EXPR:
    case SPLT_AND_EXPR:
    case SPLT_OR_EXPR:
    case SPLT_COND_EXPR:
    case SPLT_ASSIGN_EXPR:
    case SPLT_CONST_EXPR:
    case SPLT_INIT_EXPR:
    case SPLT_INIT:
    case SPLT_INIT_LIST:
    case SPLT_DESGTN:
        return "\033[95m";

    case SPLT_DECLTN:
    case SPLT_DIR_DECLTN:
    case SPLT_ABS_DEC:
    case SPLT_DIR_ABS_DEC:

    case SPLT_DEC:
    case SPLT_DIR_DEC:
    case SPLT_PTR:
    case SPLT_PARAM_DEC:
    case SPLT_ELLIPSIS:
    case SPLT_PARAM_TYPE_LIST:
    case SPLT_PARAM_LIST:
    case SPLT_ARG_LIST:
        return "\033[92m";

    /* Terminal: control keywords */
    case SPLT_WHILE:
    case SPLT_FOR:
    case SPLT_DO:

    case SPLT_IF:
    case SPLT_ELSE:
    case SPLT_SWITCH:
    case SPLT_DEFAULT:
    case SPLT_CASE:

    case SPLT_GOTO:
    case SPLT_CONTINUE:
    case SPLT_BREAK:
    case SPLT_RETURN:
        return "\033[38;5;164m";

    /* Terminal: built-in storage-class-specifiers, function-specifier, type-specifier and type-qualifiers */
    case SPLT_KWD_STRUCT:
    case SPLT_KWD_ENUM:
    case SPLT_KWD_UNION:

    case SPLT_AUTO:
    case SPLT_EXTERN:
    case SPLT_REGISTER:
    case SPLT_STATIC:
    case SPLT_TYPEDEF:

    case SPLT_KWD_CONST:
    case SPLT_RESTRICT:
    case SPLT_VOLATILE:

    case SPLT_INLINE:

    case SPLT_TYPE_VOID:
    case SPLT_TYPE_INT:
    case SPLT_TYPE_SIGNED:
    case SPLT_TYPE_UNSIGNED:
    case SPLT_TYPE_LONG:
    case SPLT_TYPE_FLOAT:
    case SPLT_TYPE_CHAR:
        return "\033[38;5;27m";

    /* Terminals: IDs */
    case SPLT_ENUM:
    case SPLT_DESG_LIST:
    case SPLT_DESG:
    case SPLT_ID:
        return "\033[38;5;81m";
    case SPLT_TYPEDEF_NAME:
        return "\33[38;5;50m";

    case SPLT_DECLTN_SPEC:
    case SPLT_TYPE_SPEC:
    case SPLT_STRG_SPEC:
    case SPLT_TYPE_QUAL_LIST:
    case SPLT_TYPE_QUAL:
    case SPLT_FUNC_SPEC:
    case SPLT_SPEC_QUAL_LIST:
    case SPLT_STRUCT_UNION_SPEC:
    case SPLT_ENUM_SPEC:
    case SPLT_MACRO_ID:
        return "\033[38;5;33m";

    case SPLT_TYPENAME:
        return "\033[38;5;47m";

    /* Terminals: punctuators */
    case SPLT_LC:
    case SPLT_RC:
    case SPLT_LP:
    case SPLT_RP:
        return "\033[38;5;220m";

    case SPLT_SEMI:
    case SPLT_COMMA:
        return "\033[38;5;1m";

    case SPLT_QM:
    case SPLT_COLON:

    case SPLT_LSB:
    case SPLT_RSB:

    case SPLT_ASSIGN:
    case SPLT_MUL_ASSIGN:
    case SPLT_DIV_ASSIGN:
    case SPLT_MOD_ASSIGN:
    case SPLT_PLUS_ASSIGN:
    case SPLT_MINUS_ASSIGN:
    case SPLT_LSHIFT_ASSIGN:
    case SPLT_RSHIFT_ASSIGN:
    case SPLT_BW_AND_ASSIGN:
    case SPLT_BW_XOR_ASSIGN:
    case SPLT_BW_OR_ASSIGN:

    case SPLT_LSHIFT:
    case SPLT_RSHIFT:
    case SPLT_BW_AND:
    case SPLT_BW_OR:
    case SPLT_BW_XOR:
    case SPLT_BW_NOT:
    case SPLT_AND:
    case SPLT_OR:
    case SPLT_NOT:

    case SPLT_SIZEOF:

    case SPLT_LT:
    case SPLT_LE:
    case SPLT_GT:
    case SPLT_GE:
    case SPLT_NE:
    case SPLT_EQ:

    case SPLT_DPLUS:
    case SPLT_DMINUS:
    case SPLT_PLUS:
    case SPLT_MINUS:
    case SPLT_ASTRK:
    case SPLT_DIV:
    case SPLT_MOD:

    case SPLT_DOT:
    case SPLT_RARROW:
        return "\033[38;5;110m";

    /* Terminals: Constant Expressions */
    case SPLT_LTR_INT:
    case SPLT_LTR_FLOAT:
    case SPLT_LTR_CHAR:
    case SPLT_LTR_STR:
    case SPLT_CONSTANT:
    case SPLT_STR_UNIT:
        return "\033[38;5;173m";

    default:
        return "\033[31m";
    }
}

const char *splc_token2str(splc_token_t type)
{
    switch (type)
    {
    /* Nonterminals */
    case SPLT_NULL:
        return "NULL-type";
    case SPLT_TRANS_UNIT:
        return "TranslationUnitDecl";
    case SPLT_EXT_DECLTN_LIST:
        return "ExternalDeclList";
    case SPLT_EXT_DECLTN:
        return "ExternalDecl";
    case SPLT_DECLTN_SPEC:
        return "DeclSpec";
    case SPLT_TYPE_SPEC:
        return "TypeSpec";
    case SPLT_STRG_SPEC:
        return "StorageSpec";
    case SPLT_TYPE_QUAL_LIST:
        return "TypeQualList";
    case SPLT_TYPE_QUAL:
        return "TypeQual";
    case SPLT_FUNC_SPEC:
        return "FunctionSpec";
    case SPLT_SPEC_QUAL_LIST:
        return "Spec-QualList";
    case SPLT_TYPENAME:
        return "typename";
    case SPLT_STRUCT_UNION_SPEC:
        return "Struct/Union-Spec";
    case SPLT_STRUCT_DECLTN_LIST:
        return "Struct/Union-DeclList";
    case SPLT_STRUCT_DECLTN:
        return "Struct/Union-Decl";
    case SPLT_STRUCT_DEC_LIST:
        return "Struct/Union-DecltrList";
    case SPLT_STRUCT_DEC:
        return "Struct/Union-Decltr";
    case SPLT_ENUM_SPEC:
        return "EnumSpec";
    case SPLT_ENUM_LIST:
        return "EnumList";
    case SPLT_ENUM:
        return "Enum";
    case SPLT_DEC:
        return "Decltr";
    case SPLT_DIR_DEC:
        return "DirectDecltr";
    case SPLT_PTR:
        return "Ptr";
    case SPLT_FUNC_DEF:
        return "FunctionDef";
    case SPLT_FUNC_DEC:
        return "FunctionDecltr";
    case SPLT_DIR_FUNC_DEC:
        return "DirectFunctionDecltr";

    case SPLT_PARAM_TYPE_LIST:
        return "ParamTypeList";
    case SPLT_PARAM_LIST:
        return "ParamList";
    case SPLT_PARAM_DEC:
        return "ParamDecltr";

    case SPLT_STMT:
        return "Stmt";
    case SPLT_GEN_STMT_LIST:
        return "GeneralStmtList";
    case SPLT_COMP_STMT:
        return "CompoundStmt";
    // case SPLT_STMT_LIST:
    //     return "StmtList";
    case SPLT_EXPR_STMT:
        return "ExprStmt";
    case SPLT_SEL_STMT:
        return "SelectionStmt";
    case SPLT_ITER_STMT:
        return "IterationStmt";
    case SPLT_FOR_LOOP_BODY:
        return "ForLoopBody";
    case SPLT_LABELED_STMT:
        return "LabeledStmt";
    case SPLT_JUMP_STMT:
        return "JumpStmt";

    /* Nonterminals: local definition */
    case SPLT_DECLTN_LIST:
        return "DeclList";
    case SPLT_DECLTN:
        return "Decl";
    case SPLT_DIR_DECLTN:
        return "DirectDecl";
    case SPLT_INIT_DEC_LIST:
        return "InitDecltrList";
    case SPLT_INIT_DEC:
        return "InitDecl";
    case SPLT_INIT:
        return "Initializer";
    case SPLT_INIT_LIST:
        return "InitializerList";
    case SPLT_DESGTN:
        return "Designation";
    case SPLT_DESG_LIST:
        return "DesignatorList";
    case SPLT_DESG:
        return "Designator";
    case SPLT_ABS_DEC:
        return "AbstractDecltr";
    case SPLT_DIR_ABS_DEC:
        return "DirectAbstractDecltr";

    case SPLT_EXPR:
        return "Expr";
    case SPLT_PRIM_EXPR:
        return "PrimaryExpr";
    case SPLT_POSTF_EXPR:
        return "PostfixExpr";
    case SPLT_UNARY_EXPR:
        return "UnaryExpr";
    case SPLT_CAST_EXPR:
        return "ExplicitCastExpr";
    case SPLT_MUL_EXPR:
        return "MulExpr";
    case SPLT_ADD_EXPR:
        return "AddExpr";
    case SPLT_SHIFT_EXPR:
        return "ShiftExpr";
    case SPLT_REL_EXPR:
        return "RelationalExpr";
    case SPLT_EQ_EXPR:
        return "EqualityExpr";
    case SPLT_BW_AND_EXPR:
        return "BitwiseANDExpr";
    case SPLT_BW_XOR_EXPR:
        return "BitwiseXORExpr";
    case SPLT_BW_OR_EXPR:
        return "BitwiseORExpr";
    case SPLT_AND_EXPR:
        return "LogicalANDExpr";
    case SPLT_OR_EXPR:
        return "LogicalORExpr";
    case SPLT_COND_EXPR:
        return "ConditionalExpr";
    case SPLT_ASSIGN_EXPR:
        return "AssignmentExpr";
    case SPLT_CONST_EXPR:
        return "ConstExpr";
    case SPLT_INIT_EXPR:
        return "InitExpr";
    case SPLT_CONSTANT:
        return "Constant";

    case SPLT_ARG_LIST:
        return "ArgList";

    /* Terminal: Keywords */
    case SPLT_KWD_STRUCT:
        return "struct";
    case SPLT_KWD_ENUM:
        return "enum";
    case SPLT_KWD_UNION:
        return "union";

    case SPLT_AUTO:
        return "auto";
    case SPLT_EXTERN:
        return "extern";
    case SPLT_REGISTER:
        return "register";
    case SPLT_STATIC:
        return "static";
    case SPLT_TYPEDEF:
        return "typedef";

    case SPLT_KWD_CONST:
        return "const";
    case SPLT_RESTRICT:
        return "restrict";
    case SPLT_VOLATILE:
        return "volatile";
    case SPLT_ELLIPSIS:
        return "VAR_ARG ellipsis";

    case SPLT_INLINE:
        return "inline";

    case SPLT_WHILE:
        return "while";
    case SPLT_FOR:
        return "for";
    case SPLT_DO:
        return "do";

    case SPLT_IF:
        return "if";
    case SPLT_ELSE:
        return "else";
    case SPLT_SWITCH:
        return "switch";
    case SPLT_DEFAULT:
        return "default";
    case SPLT_CASE:
        return "case";

    case SPLT_GOTO:
        return "goto";
    case SPLT_CONTINUE:
        return "continue";
    case SPLT_BREAK:
        return "break";
    case SPLT_RETURN:
        return "return";

    /* Terminals: Punctuations */
    case SPLT_LC:
        return "LC";
    case SPLT_RC:
        return "RC";
    case SPLT_LP:
        return "LP";
    case SPLT_RP:
        return "RP";

    case SPLT_SEMI:
        return "Semi";
    case SPLT_COMMA:
        return "Comma";

    /* Operators */
    case SPLT_QM:
        return "QuestionMark";
    case SPLT_COLON:
        return "Colon";

    case SPLT_LSB:
        return "LSB";
    case SPLT_RSB:
        return "RSB";

    case SPLT_ASSIGN:
        return "Assign";
    case SPLT_MUL_ASSIGN:
        return "MulAssign";
    case SPLT_DIV_ASSIGN:
        return "DivAssign";
    case SPLT_MOD_ASSIGN:
        return "ModAssign";
    case SPLT_PLUS_ASSIGN:
        return "PlusAssign";
    case SPLT_MINUS_ASSIGN:
        return "MinusAssign";
    case SPLT_LSHIFT_ASSIGN:
        return "LShiftAssign";
    case SPLT_RSHIFT_ASSIGN:
        return "RShiftAssign";
    case SPLT_BW_AND_ASSIGN:
        return "BwAndAssign";
    case SPLT_BW_XOR_ASSIGN:
        return "BwXorAssign";
    case SPLT_BW_OR_ASSIGN:
        return "BwOrAssign";

    case SPLT_LSHIFT:
        return "LShift";
    case SPLT_RSHIFT:
        return "RShift";
    case SPLT_BW_AND:
        return "BwAnd";
    case SPLT_BW_OR:
        return "BwOr";
    case SPLT_BW_XOR:
        return "BwXor";
    case SPLT_BW_NOT:
        return "BwNot";
    case SPLT_AND:
        return "And";
    case SPLT_OR:
        return "Or";
    case SPLT_NOT:
        return "Not";

    case SPLT_SIZEOF:
        return "sizeof";

    case SPLT_LT:
        return "LT";
    case SPLT_LE:
        return "LE";
    case SPLT_GT:
        return "GT";
    case SPLT_GE:
        return "GE";
    case SPLT_NE:
        return "NE";
    case SPLT_EQ:
        return "EQ";

    case SPLT_DPLUS:
        return "DoublePlus";
    case SPLT_DMINUS:
        return "DoubleMinus";
    case SPLT_PLUS:
        return "Plus";
    case SPLT_MINUS:
        return "Minus";
    case SPLT_ASTRK:
        return "Asterisk";
    case SPLT_DIV:
        return "Div";
    case SPLT_MOD:
        return "Mod";

    case SPLT_DOT:
        return "Dot";
    case SPLT_RARROW:
        return "RArrow";

    case SPLT_TYPE_VOID:
        return "VoidType";
    case SPLT_TYPE_INT:
        return "IntType";
    case SPLT_TYPE_SIGNED:
        return "SignedType";
    case SPLT_TYPE_UNSIGNED:
        return "UnsignedType";
    case SPLT_TYPE_LONG:
        return "LongType";
    case SPLT_TYPE_FLOAT:
        return "FloatType";
    case SPLT_TYPE_CHAR:
        return "CharType";

    case SPLT_ID:
        return "ID";
    case SPLT_TYPEDEF_NAME:
        return "TypedefName";

        /* Terminals: literals */

    case SPLT_LTR_INT:
        return "IntLiteral";
    case SPLT_LTR_FLOAT:
        return "FloatLiteral";
    case SPLT_LTR_CHAR:
        return "CharLiteral";
    case SPLT_LTR_STR:
        return "StringLiteral";
    case SPLT_STR_UNIT:
        return "StringUnit";

    /* Nonterminals: Macro Expressions */
    case SPLT_MACRO_MNTPT:
        return "ASTMacroMountpoint";

    case SPLT_MACRO_ID:
        return "ASTMacroID";

    default:
        return "InvalidName";
    }
}

splc_trans_unit splc_create_empty_trans_unit()
{
    splc_trans_unit unit = (splc_trans_unit)malloc(sizeof(splc_trans_unit_struct));
    SPLC_ALLOC_PTR_CHECK(unit, "out of memory when creating translation unit");
    unit->global_symtable = NULL;
    unit->root = NULL;
    unit->envs = NULL;
    unit->nenvs = 0;
    unit->err_count = 0;
    unit->warn_count = 0;
    return unit;
}

splc_trans_unit splc_create_trans_unit()
{
    splc_trans_unit unit = splc_create_empty_trans_unit();
    unit->global_symtable = lut_new_table(0);
    return unit;
}

lut_table splc_push_symtable(splc_trans_unit tunit, int scope)
{
    lut_table *newarr = (lut_table *)realloc(tunit->envs, (tunit->nenvs + 1) * sizeof(lut_table));
    SPLC_ALLOC_PTR_CHECK(newarr, "cannot allocate new symbol table for internal scope");
    tunit->envs = newarr;
    tunit->envs[tunit->nenvs] = lut_new_table(scope);
    tunit->nenvs++;
    return tunit->envs[tunit->nenvs - 1];
}

lut_table splc_pop_symtable(splc_trans_unit tunit)
{
    if (tunit->nenvs == 0)
        return NULL;
    lut_table result = tunit->envs[tunit->nenvs - 1];
    lut_table *newarr = NULL;
    if (tunit->nenvs > 1)
    {
        newarr = (lut_table *)realloc(tunit->envs, (tunit->nenvs - 1) * sizeof(lut_table));
        SPLC_ALLOC_PTR_CHECK(newarr, "cannot deallocate symbol table for internal scope");
    }
    else
    {
        free(tunit->envs);
    }
    tunit->envs = newarr;
    tunit->nenvs--;
    return result;
}