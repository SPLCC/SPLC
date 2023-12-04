#ifndef TYPE_H
#define TYPE_H

#include "splcdef.h"
#include "ast.h"

int check_specifier_list_validity(splc_trans_unit, int root_env, int struct_decl_env, ast_node specs);

int type_compare(splc_trans_unit, int root_env, int struct_decl_env, ast_node decltn1, ast_node decltn2);

int type_promote(splc_trans_unit, int root_env, int struct_decl_env, ast_node decltn1, ast_node decltn2);

#endif