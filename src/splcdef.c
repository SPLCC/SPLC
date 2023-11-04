#include "splcdef.h"
#include "utils.h"

int err_flag = 0;

char *splc_loc2str(splc_loc location)
{
    char *buffer = NULL;
    if (SPLC_IS_LOC_INVALID(location))
    {
        buffer = strdup("invalid");
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
    case AST_NULL:
        return "\033[31m";

    case AST_PROGRAM:
        return "\033[38;5;51m";

    /* Nonterminals: Macro Expressions */
    case AST_MACRO_MNTPT:
        return "\033[38;5;141m";

    case AST_FUNC_DEC:
        return "\033[38;5;229m";

    /* Nonterminals: statements */
    case AST_EXT_DEF_LIST:
    case AST_EXT_DEF:
    case AST_EXT_DEC_LIST:
    case AST_COMP_STMT:
    case AST_STMT_LIST:
    case AST_STMT:
    case AST_FOR_LOOP_BODY:
    case AST_DEF_LIST:
    case AST_DEF:
    case AST_DEC_LIST:
    case AST_DEC:
        return "\033[38;5;27m";

    case AST_EXP:
    case AST_STREXP:
        return "\033[38;5;40m";

    case AST_VAR_DEC:
    case AST_PARAM_DEC:
    case AST_VAR_LIST:
    case AST_ARGS:
        return "\033[38;5;81m";

    case AST_LC:
    case AST_RC:
    case AST_LP:
    case AST_RP:
    case AST_LSB:
    case AST_RSB:
        return "\033[38;5;220m";


    /* Terminal: Keywords */
    case AST_STRUCT:
    case AST_IF:
    case AST_ELSE:
    case AST_WHILE:
    case AST_FOR:
    case AST_RETURN:
        return "\033[38;5;164m";

    /* Terminals: IDs */
    case AST_BUILTIN_TYPE:
    case AST_ID:
        return "\033[38;5;81m";
        
    case AST_SPECIFIER:
    case AST_STRUCT_SPECIFIER:
    case AST_MACRO_ID:
        return "\033[38;5;33m";

    /* Terminals: Constant Expressions */
    case AST_INT:
    case AST_FLOAT:
    case AST_CHAR:
    case AST_STR:
        return "\033[38;5;173m";
        
    /* Terminals: punctuations & operators */
    case AST_SEMI:
    case AST_COMMA:
    case AST_ASSIGN:
    case AST_AND:
    case AST_OR:
    case AST_LT:
    case AST_LE:
    case AST_GT:
    case AST_GE:
    case AST_NE:
    case AST_EQ:
    case AST_PLUS:
    case AST_MINUS:
    case AST_MUL:
    case AST_DIV:
    case AST_NOT:
    case AST_DOT:
        return "\033[38;5;110m";
    default:
        return "\033[0m";
    }
}

const char *splc_token2str(splc_token_t type)
{
    switch (type)
    {
    /* Nonterminals */
    case AST_NULL:
        return "NULL-TYPE";

    case AST_PROGRAM:
        return "Program";

    case AST_EXT_DEF_LIST:
        return "ExtDefList";

    case AST_EXT_DEF:
        return "ExtDef";

    case AST_EXT_DEC_LIST:
        return "ExtDecList";

    case AST_SPECIFIER:
        return "Specifier";

    case AST_STRUCT_SPECIFIER:
        return "StructSpecifier";

    case AST_VAR_DEC:
        return "VarDec";

    case AST_FUNC_DEC:
        return "FunDec";

    case AST_VAR_LIST:
        return "VarList";

    case AST_PARAM_DEC:
        return "ParamDec";

    case AST_COMP_STMT:
        return "CompSt";

    case AST_STMT_LIST:
        return "StmtList";

    case AST_STMT:
        return "Stmt";

    case AST_FOR_LOOP_BODY:
        return "For Loop Body";

    /* Nonterminals: local definition */
    case AST_DEF_LIST:
        return "DefList";

    case AST_DEF:
        return "Def";

    case AST_DEC_LIST:
        return "DecList";

    case AST_DEC:
        return "Dec";

    case AST_EXP:
        return "Exp";

    case AST_ARGS:
        return "Args";

    case AST_ASSIGN:
        return "ASSIGN";

    case AST_AND:
        return "AND";

    case AST_OR:
        return "ExtDefList";

    case AST_LT:
        return "LT";

    case AST_LE:
        return "LE";

    case AST_GT:
        return "GT";

    case AST_GE:
        return "GE";

    case AST_NE:
        return "NE";

    case AST_EQ:
        return "EQ";

    case AST_PLUS:
        return "PLUS";

    case AST_MINUS:
        return "MINUS";

    case AST_MUL:
        return "MUL";

    case AST_DIV:
        return "DIV";

    case AST_LC:
        return "LC";

    case AST_RC:
        return "RC";

    case AST_LP:
        return "LP";

    case AST_RP:
        return "RP";

    case AST_NOT:
        return "NOT";

    case AST_DOT:
        return "DOT";

    case AST_LSB:
        return "LB";

    case AST_RSB:
        return "RB";

    /* Terminals: Punctuations */
    case AST_SEMI:
        return "SEMI";

    case AST_COMMA:
        return "COMMA";

    /* Terminal: Keywords */
    case AST_STRUCT:
        return "STRUCT";

    case AST_IF:
        return "IF";

    case AST_ELSE:
        return "ELSE";

    case AST_WHILE:
        return "WHILE";

    case AST_FOR:
        return "FOR";

    case AST_RETURN:
        return "RETURN";

    /* Terminals: Constant Expressions */
    case AST_BUILTIN_TYPE:
        return "TYPE";

    case AST_ID:
        return "ID";

    case AST_INT:
        return "INT";

    case AST_FLOAT:
        return "FLOAT";

    case AST_CHAR:
        return "CHAR";

    case AST_STREXP:
        return "Compound String Expr";

    case AST_STR:
        return "String";

    /* Nonterminals: Macro Expressions */
    case AST_MACRO_MNTPT:
        return "AST Macro Mountpoint";

    case AST_MACRO_ID:
        return "AST Macro ID";

    default:
        return "UNRECOGNIZED";
    }
}