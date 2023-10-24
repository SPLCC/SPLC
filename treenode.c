#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>
#include "treenode.h"

void addChild(struct TreeNode* parent, struct TreeNode* child)
{
    parent->child = (struct TreeNode**) realloc(parent->child, (parent->num_child + 1) * sizeof(struct TreeNode*));
    parent->child[parent->num_child] = child;
    parent->num_child++;
}

struct TreeNode* createNode(const char* name, int num_child, ...)
{
    struct TreeNode* node = (struct TreeNode*)malloc(sizeof(struct TreeNode));
    node->name = strdup(name);
    node->num_child = 0;
    node->type = 0;
    va_list args;
    va_start(args, num_child);
    for(int i = 0; i < num_child; i++)
    {
        struct TreeNode* child = va_arg(args, struct TreeNode*);
        if(child==NULL)  continue;
        addChild(node, child);
    }
    va_end(args);
    return node;
}

void printParseTree(struct TreeNode* node, int level) {
    for (int i = 0; i < level; i++) {
        printf("  ");
    }
    printf("%s", node->name);
    if(node->type==1)
    {
        printf(": %s",node->string_val);
    }
    else if(node->type==2){
        printf(": %d",node->int_val);
    }
    else if(node->type==3){
        printf(": %f",node->float_val);
    }
    puts("");
    for (int i = 0; i < node->num_child; i++) {
        printParseTree(node->child[i], level + 1);
    }
}