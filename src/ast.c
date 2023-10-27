#include "ast.h"

#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>

ast_node create_empty_node()
{
    ast_node node = (ast_node)malloc(sizeof(ast_node_struct));
    node->type = AST_NULL;
    node->entry = NULL;
    node->children = NULL;
    node->num_child = 0;
    node->lineno = 0;
    node->colno = 0;
    return node;
}

ast_node create_leaf_node(spl_token_t type)
{
    ast_node node = create_empty_node();
    node->type = type;
    return node;
}

ast_node add_child(ast_node parent, ast_node child)
{
    parent->children = (ast_node *)realloc(parent->children, (parent->num_child + 1) * sizeof(ast_node_struct));
    parent->children[parent->num_child] = child;
    ++(parent->num_child);
}

ast_node create_parent_node(spl_token_t type, int lineno, size_t num_child, ...)
{
    ast_node node = create_empty_node();
    node->type = type;
    node->num_child = 0;
    node->lineno = lineno;

    va_list args;
    va_start(args, num_child);

    for (size_t i = 0; i < num_child; ++i)
    {
        ast_node child = va_arg(args, ast_node);
        if (child == NULL || child->type == AST_NULL)
            continue;
        add_child(node, child);
    }
    va_end(args);
    return node;
}

void builtin_print_ast(ast_node node, int level)
{
    for (int i = 0; i < level; i++)
    {
        printf("  ");
    }

    printf("%s", get_spl_token_string(node->type));

    switch (node->type)
    {
    case AST_BUILTIN_TYPE:
        printf(": %s", (char *)node->val);
        break;

    case AST_ID:
        printf(": %s", (char *)node->val);
        break;

    case AST_INT:
        printf(": %d", node->int_val);
        break;
    case AST_FLOAT:
        printf(": %g", node->float_val);
        break;
    case AST_CHAR:
        printf(": %s", (char *)node->val);
        break;

    default:
        break;
    }

    if (node->lineno > 0)
    {
        printf(" (%d)", node->lineno);
    }
    printf("\n");
    for (int i = 0; i < node->num_child; i++)
    {
        builtin_print_ast(node->children[i], level + 1);
    }
}

void print_ast(ast_node root)
{
    builtin_print_ast(root, 0);
}