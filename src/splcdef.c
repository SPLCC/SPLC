#include "splcdef.h"
#include "utils.h"

int err_flag = 0;

char *splc_loc2str(splc_loc location)
{
    char *buffer = NULL;
    if (SPLC_IS_LOC_INVALID(location))
    {
        buffer = strdup("{invalid loc}");
    }
    else
    {
        size_t needed = 1 + snprintf(NULL, 0, "{%d, (%d, %d)->(%d, %d)}", location.fid,
                                     location.linebegin, location.colbegin, location.lineend, location.colend);
        buffer = (char *)malloc(needed * sizeof(char));
        if (buffer == NULL)
            splcfail("cannot allocate memory for location printing");
        sprintf(buffer, "{%d, (%d, %d)->(%d, %d)}", location.fid, location.linebegin, location.colbegin,
                location.lineend, location.colend);
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
        return "\033[31m";

    case SPLT_TRANS_UNIT:
        return "\033[38;5;51m";

    /* Nonterminals: Macro Expressions */
    case SPLT_MACRO_MNTPT:
        return "\033[38;5;141m";

    case SPLT_FUNC_DEC:
    case SPLT_DIR_FUNC_DEC:
        return "\033[38;5;229m";

    /* Nonterminals: statements */
    case SPLT_EXT_DECLTN_LIST:
    case SPLT_EXT_DECLTN:
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

    case SPLT_DECLTN_LIST:
    case SPLT_DECLTN:
    case SPLT_INIT_DEC_LIST:
    case SPLT_INIT_DEC:
    case SPLT_ABS_DEC:
    case SPLT_DIR_ABS_DEC:
        return "\033[38;5;27m";

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
        return "\033[38;5;40m";

    case SPLT_DEC:
    case SPLT_DIR_DEC:
    case SPLT_PTR:
    case SPLT_PARAM_DEC:
    case SPLT_VAR_LIST:
    case SPLT_ARG_LIST:
        return "\033[38;5;81m";

    /* Terminal: control keywords */
    case SPLT_STRUCT:
    case SPLT_UNION:

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

    /* Terminal: built-in types */
    case SPLT_TYPE_VOID:
    case SPLT_TYPE_INT:
    case SPLT_TYPE_FLOAT:
    case SPLT_TYPE_CHAR:
        return "\033[38;5;27m";

    /* Terminals: IDs */
    case SPLT_ID:
        return "\033[38;5;81m";
        
    case SPLT_TYPE_SPEC:
    case SPLT_TYPENAME:
    case SPLT_STRUCT_SPECIFIER:
    case SPLT_MACRO_ID:
        return "\033[38;5;33m";

        
    /* Terminals: punctuators */
    case SPLT_LC:
    case SPLT_RC:
    case SPLT_LP:
    case SPLT_RP:
    case SPLT_LSB:
    case SPLT_RSB:
        return "\033[38;5;220m";

    case SPLT_SEMI:
    case SPLT_COMMA:
        return "\033[38;5;1m";

    case SPLT_QM:
    case SPLT_COLON:

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
    case SPLT_STR:
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
        return "Translation Unit";
    case SPLT_EXT_DECLTN_LIST:
        return "External Declaration List";
    case SPLT_EXT_DECLTN:
        return "External Declaration";
    case SPLT_TYPE_SPEC:
        return "Specifier";
    case SPLT_TYPENAME:
        return "typename";
    case SPLT_STRUCT_SPECIFIER:
        return "Struct Specifier";
    case SPLT_DEC:
        return "Declarator";
    case SPLT_DIR_DEC:
        return "Direct Declarator";
    case SPLT_PTR:
        return "Pointer";
    case SPLT_FUNC_DEC:
        return "FunDec";
    case SPLT_DIR_FUNC_DEC:
        return "Direct FunDec";

    case SPLT_VAR_LIST:
        return "VarList";
    case SPLT_PARAM_DEC:
        return "ParamDec";

    case SPLT_STMT:
        return "Stmt";
    case SPLT_GEN_STMT_LIST:
        return "General Stmt List";
    case SPLT_COMP_STMT:
        return "CompStmt";
    // case SPLT_STMT_LIST:
    //     return "StmtList";
    case SPLT_EXPR_STMT:
        return "Expression Stmt";
    case SPLT_SEL_STMT:
        return "Selection Stmt";
    case SPLT_ITER_STMT:
        return "Iteration Stmt";
    case SPLT_FOR_LOOP_BODY:
        return "For Loop Body";
    case SPLT_LABELED_STMT:
        return "Labeled Stmt";
    case SPLT_JUMP_STMT:
        return "Jump Stmt";

    /* Nonterminals: local definition */
    case SPLT_DECLTN_LIST:
        return "Declaration List";
    case SPLT_DECLTN:
        return "Declaration";
    case SPLT_INIT_DEC_LIST:
        return "Init Declarator List";
    case SPLT_INIT_DEC:
        return "Init Declarator";
    case SPLT_ABS_DEC:
        return "Abstract Dec";
    case SPLT_DIR_ABS_DEC:
        return "Direct Abstract Dec";

    case SPLT_EXPR:
        return "Expr";
    case SPLT_PRIM_EXPR:
        return "Primary Expr";
    case SPLT_POSTF_EXPR:
        return "Postfix Expr";
    case SPLT_UNARY_EXPR:
        return "Unary Expr";
    case SPLT_CAST_EXPR:
        return "Cast Expr";
    case SPLT_MUL_EXPR:
        return "Mul Expr";
    case SPLT_ADD_EXPR:
        return "Add Expr";
    case SPLT_SHIFT_EXPR:
        return "Shift Expr";
    case SPLT_REL_EXPR:
        return "Relational Expr";
    case SPLT_EQ_EXPR:
        return "Equality Expr";
    case SPLT_BW_AND_EXPR:
        return "Bitwise AND Expr";
    case SPLT_BW_XOR_EXPR:
        return "Bitwise XOR Expr";
    case SPLT_BW_OR_EXPR:
        return "Bitwise OR Expr";
    case SPLT_AND_EXPR:
        return "Logical AND Expr";
    case SPLT_OR_EXPR:
        return "Logical OR Expr";
    case SPLT_COND_EXPR:
        return "Conditional Expr";
    case SPLT_ASSIGN_EXPR:
        return "Assignment Expr";
    case SPLT_CONST_EXPR:
        return "Const Expr";
    case SPLT_INIT_EXPR:
        return "Init Expr";
    case SPLT_CONSTANT:
        return "Constant";

    case SPLT_ARG_LIST:
        return "Arg List";

    /* Terminal: Keywords */
    case SPLT_STRUCT:
        return "struct";
    case SPLT_UNION:
        return "union";

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
    case SPLT_LSB:
        return "LSB";
    case SPLT_RSB:
        return "RSB";

    case SPLT_SEMI:
        return "SEMI";
    case SPLT_COMMA:
        return "COMMA";

    case SPLT_QM:
        return "QUESTION MARK";
    case SPLT_COLON:
        return "COLON";

    /* Assignment Operators */
    case SPLT_ASSIGN:
        return "ASSIGN";
    case SPLT_MUL_ASSIGN:
        return "MUL_ASSIGN";
    case SPLT_DIV_ASSIGN:
        return "DIV_ASSIGN";
    case SPLT_MOD_ASSIGN:
        return "MOD_ASSIGN";
    case SPLT_PLUS_ASSIGN:
        return "PLUS_ASSIGN";
    case SPLT_MINUS_ASSIGN:
        return "MINUS_ASSIGN";
    case SPLT_LSHIFT_ASSIGN:
        return "LSHIFT_ASSIGN";
    case SPLT_RSHIFT_ASSIGN:
        return "RSHIFT_ASSIGN";
    case SPLT_BW_AND_ASSIGN:
        return "BW_AND_ASSIGN";
    case SPLT_BW_XOR_ASSIGN:
        return "BW_XOR_ASSIGN";
    case SPLT_BW_OR_ASSIGN:
        return "BW_OR_ASSIGN";

    case SPLT_LSHIFT:
        return "LSHIFT";
    case SPLT_RSHIFT:
        return "RSHIFT";
    case SPLT_BW_AND:
        return "bitwise AND";
    case SPLT_BW_OR:
        return "bitwise OR";
    case SPLT_BW_XOR:
        return "bitwise XOR";
    case SPLT_BW_NOT:
        return "bitwise NOT";
    case SPLT_AND:
        return "AND";
    case SPLT_OR:
        return "OR";
    case SPLT_NOT:
        return "NOT";

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

    case SPLT_PLUS:
        return "PLUS";
    case SPLT_MINUS:
        return "MINUS";
    case SPLT_ASTRK:
        return "ASTRK";
    case SPLT_DIV:
        return "DIV";
    case SPLT_MOD:
        return "MOD";

    case SPLT_DOT:
        return "DOT";
    case SPLT_RARROW:
        return "RARROW";

    case SPLT_TYPE_VOID:
        return "type: void";
    case SPLT_TYPE_INT:
        return "type: int";
    case SPLT_TYPE_FLOAT:
        return "type: float";
    case SPLT_TYPE_CHAR:
        return "type: char";

    case SPLT_ID:
        return "ID";

    /* Terminals: literals */

    case SPLT_LTR_INT:
        return "integer literal";
    case SPLT_LTR_FLOAT:
        return "float literal";
    case SPLT_LTR_CHAR:
        return "char literal";
    case SPLT_LTR_STR:
        return "String Literal";
    case SPLT_STR:
        return "Str Unit";

    /* Nonterminals: Macro Expressions */
    case SPLT_MACRO_MNTPT:
        return "AST Macro Mountpoint";

    case SPLT_MACRO_ID:
        return "AST Macro ID";

    default:
        return "unspecified name";
    }
}