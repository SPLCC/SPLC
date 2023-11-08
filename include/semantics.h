#ifndef SEMANTIC_H
#define SEMANTIC_H

#include "splcdef.h"
#include "ast.h"
#include "lut.h"

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

void sem_analyze(lut_table symbol_table, ast_node root);

#endif // SEMANTIC_H