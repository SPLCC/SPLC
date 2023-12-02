#ifndef SEMANTICS_H
#define SEMANTICS_H

#include "splcdef.h"
#include "ast.h"
#include "lut.h"

/* expression type enum */
typedef enum sem_expr_type sem_expr_t;
enum sem_expr_type 
{
    EXPR_NULL = 0x00000000,
    EXPR_INT,
    EXPR_FLOAT,
    EXPR_CHAR
};

/* EXPERIMENTAL: Register typedef names for the following AST nodes:
   - direct-declaration
   - function-definition
   
   Note: registration will be done IFF keyword `typedef` exists.
    */
void sem_register_typedef(const ast_node node);

/* EXPERIMENTAL
    */
int sem_test_typedef_name(const char *name);

void sem_track_scope_root(ast_node node);

void sem_analyze(splc_trans_unit tunit);

void sem_ast_search(ast_node node, ast_node fa_node, splc_trans_unit tunit, int new_sym_table, splc_entry_t decl_entry_type, splc_entry_t decl_extra_type, const char* decl_spec_type, int in_struct, int in_expr);

sem_expr_t sem_ast_expr_process(const ast_node node, splc_trans_unit tunit);

#endif /* SEMANTICS_H */