/* This file defines the Abstract Syntax Tree that will be used inside the
 * parser. */
#ifndef AST_H
#define AST_H
#include "lut.h"

/* Struct definitions */

typedef ast_node_struct ast_node;

typedef struct ast_node_struct {
    char *name;
    struct TreeNode **child;
    int num_child;
    int type;

} ast_node_struct;

/* Function definitions */

ast_node create_leaf_node();

ast_node create_node()

#endif