#ifndef TYPE_H
#define TYPE_H

#include "splcdef.h"
#include "ast.h"

typedef struct type_entry_struct type_entry_struct;

typedef type_entry_struct *type_entry_t;

struct type_entry_struct {
    size_t num_specifiers;
    ast_node *specifiers;
    ast_node declarator;
};

type_entry_t type_new_entry();

type_entry_t parse_declaration_specifier_list(type_entry_t type, ast_node node);

#endif