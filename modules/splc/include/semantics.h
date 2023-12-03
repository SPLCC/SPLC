#ifndef SEMANTICS_H
#define SEMANTICS_H

#include "splcdef.h"
#include "ast.h"
#include "lut.h"

/* expression type enum */
typedef struct sem_expr_entry_struct
{
    splc_entry_t extra_type;       /* Extra type of this entry */
    const char *spec_type;         /* Specific type of this entry */
    char *id;                      /* The name of the entry, if any */
} sem_expr_entry_struct;


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

#endif /* SEMANTICS_H */