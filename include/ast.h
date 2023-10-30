/* This file defines the Abstract Syntax Tree that will be used inside the
 * parser. */
#ifndef AST_H
#define AST_H

#include "spldef.h"

/* Struct definitions */

typedef struct ast_node_struct *ast_node;

typedef struct ast_node_struct
{
    spl_token_t type;   /* Type of this node */
    lut_entry entry;    /* The entry of this ID, if possible. Else NULL. AST has no control of this entry, and thus cannot free it. */
    ast_node *children; /* Array of children */
    size_t num_child;   /* Number of children */

    int fid;    /* Which file is this? */
    int lineno; /* line number */
    int colno;  /* column number */
    spl_loc location; /* Location of this token */

    union {
        void *val;
        unsigned long ulong_val; /* Interpret the value as integer */
        float float_val;  /* Interpret the value as float  */
    };
} ast_node_struct;

/* Function definitions */
/* Create an empty node that has all its fields initialized to empty/NULL. */
ast_node create_empty_node();

/* Create a leaf node given type. */
ast_node create_leaf_node(spl_token_t type);

/* Add a child to a node. Uses `realloc` to allocate memory. */
void add_child(ast_node parent, ast_node child);

/* Create a parent node, given a list of child nodes */
ast_node create_parent_node(spl_token_t type, int lineno, size_t num_child, ...);

/* Release the entire tree */
void release_tree(ast_node root);

/* Duplicate a single node */
ast_node duplicate_node(ast_node node);

/* Duplicate the entire tree */
ast_node duplicate_tree(ast_node root);

/* Substitute all macro mount points inside the given tree */
void invoke_macro_subtitution(ast_node root);

/* Print the Syntax Tree */
void print_ast(ast_node root);

#endif