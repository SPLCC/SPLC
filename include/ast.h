/* This file defines the Abstract Syntax Tree that will be used inside the parser. */
#ifndef AST_H
#define AST_H

#include "splcdef.h"

/* Struct definitions */

typedef struct ast_node_struct *ast_node;

typedef struct ast_node_struct
{
    splc_token_t type; /* Type of this node */
    lut_entry entry; /* The entry of this ID, if possible. Else NULL. AST has no control of this entry, and thus cannot
                        free it. */
    ast_node *children; /* Array of children */
    size_t num_child;   /* Number of children */

    splc_loc location; /* Location of this token. This location will cover the consecutive nodes in the first file only. */

    union {
        void *val;
        unsigned long ulong_val; /* Interpret the value as integer */
        float float_val;         /* Interpret the value as float  */
    };
} ast_node_struct;

/* Function definitions */
/* Create an empty node that has all its fields initialized to empty/NULL. */
ast_node create_empty_node();

void destruct_node(ast_node node);

/* Create a leaf node given type. */
ast_node create_leaf_node(const splc_token_t type, const splc_loc location);

/* Add a child to a node. Uses `realloc` to allocate memory. */
ast_node add_child(ast_node parent, ast_node child);

/* Add a child to a node. Uses `realloc` to allocate memory. */
ast_node add_children(ast_node parent, size_t num_child, ...);

/* Create a parent node, given a list of child nodes. The location of parent node will be that of the first-encountered
 * child node. */
ast_node create_parent_node(const splc_token_t type, size_t num_child, ...);

/* Get the starting location of this node */
splc_loc get_startloc(const ast_node node);

/* Get the ending location of this node */
splc_loc get_endloc(const ast_node node);

/* Release the entire AST */
void release_tree(ast_node root);

/* Preprocess an AST by eliminating all punctuators */
void preprocess_ast(ast_node root);

/* Duplicate a single node */
ast_node duplicate_node(ast_node node);

/* Duplicate the entire AST */
ast_node duplicate_tree(ast_node root);

/* Substitute all macro mount points inside the given AST.
   When subsituting macro functions, the following requirement holds:
   - Once the target macro function has been substituted, it is not possible to substitute the outer part again. */
void invoke_macro_subtitution(ast_node root);

/* Print the Syntax Tree */
void print_ast(const ast_node root);

/* Macros */

#define SPLC_AST_GET_STARTLOC(node) (get_startloc(node))
#define SPLC_AST_GET_ENDLOC(node) (get_endloc(node))
#define SPLC_AST_IGNORE_NODE(node) ((node) == NULL || (node)->type == SPLT_NULL || (!splc_enable_ast_punctuators && SPLT_IS_PUNCTUATOR((node)->type)))
#define SPLT_AST_REQUIRE_VAL_FREE(x) SPLT_IS_VAL_ALLOCATED(x)

#endif