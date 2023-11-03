#include "splcdef.h"

int err_flag = 0;

const char *get_splc_token_string(splc_token_t type) {
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

    case AST_MACRO_MNTPT:
        return "AST Macro Mountpoint";
    
    case AST_MACRO_ID:
        return "AST Macro ID";
    
    default:
        return "UNRECOGNIZED";
    }
}