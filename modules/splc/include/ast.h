/* This file defines the Abstract Syntax Tree that will be used inside the parser. */
#ifndef AST_H
#define AST_H

#include "splcdef.h"
#include "lut.h"

/* Struct definitions */

typedef struct ast_node_struct *ast_node;

typedef struct ast_node_struct
{
    splc_token_t type;  /* Type of this node */
    lut_table symtable; /* If this is a function declaration/definition, or a compound statement, 
                           `symtable` stores the underlying symbol table. */

    ast_node father;    /* Father of this node. A node has no control of its father. */
    ast_node *children; /* Array of children */
    size_t num_child;   /* Number of children */

    splc_loc location; /* Location of this token. This location will cover 
                          the consecutive nodes in the first file only. */

    union {
        void *val;
        unsigned long long ull_val; /* Interpret the value as integer */
        float float_val;            /* Interpret the value as float  */
    };
} ast_node_struct;

/* Function definitions */
/* Create an empty node that has all its fields initialized to empty/NULL. */
ast_node ast_create_empty_node();

/* Create a leaf node given type. */
ast_node ast_create_leaf_node(const splc_token_t type, const splc_loc location);

/* Add a child to a node. Uses `realloc` to allocate memory. */
ast_node ast_add_child(ast_node parent, ast_node child);

/* Add a child to a node. Interally, this is done by calling `ast_add_child`. */
ast_node ast_add_children(ast_node parent, size_t num_child, ...);

/* Create a parent node, given a list of child nodes. The location of parent node will be that of the first-encountered
 * child node. Interally, this is done by calling `ast_add_child`. */
ast_node ast_create_parent_node(const splc_token_t type, size_t num_child, ...);

/* Get the starting location of this node */
splc_loc ast_get_startloc(const ast_node node);

/* Get the ending location of this node */
splc_loc ast_get_endloc(const ast_node node);

/* Release the entire AST */
void ast_release_node(ast_node *root);

/* Preprocess an AST:
   - Eliminate all punctuators. The root node would not be eliminated. */
void ast_preprocess(ast_node root);

/* Recusively copy a single tree rooted at `node`. The underlying symbol table will get a shallow copy. */
ast_node ast_deep_copy(ast_node node);

/* Substitute all macro mount points inside the given AST.
   When subsituting macro functions, the following requirement holds:
   - Once the target macro function has been substituted, it is not possible to substitute the outer part again. */
void ast_invoke_macro_subtitution(ast_node root);

/* Print a single node. No line break will be appended. */
void ast_print_single_node(const ast_node node);

/* Print the Syntax Tree */
void ast_print(const ast_node root);

/* Macros */

#define SPLC_AST_PRINT_COLORED(x)                                                                                      \
    do                                                                                                                 \
    {                                                                                                                  \
        if (!splcf_no_ast_color)                                                                                       \
            printf("%s", (x));                                                                                         \
    } while (0)

#define SPLC_AST_GET_STARTLOC(node) (ast_get_startloc(node))
#define SPLC_AST_GET_ENDLOC(node) (ast_get_endloc(node))
#define SPLC_AST_IGNORE_NODE(node)                                                                                     \
    ((node) == NULL || (node)->type == SPLT_NULL || (!splcf_enable_ast_punctuators && SPLT_IS_PUNCTUATOR((node)->type)))
#define SPLT_AST_REQUIRE_VAL_FREE(x) SPLT_IS_VAL_ALLOCATED(x)

#endif /* AST_H */