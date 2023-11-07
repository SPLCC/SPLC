#ifndef SEMANTIC_H
#define SEMANTIC_H

#include "splcdef.h"
#include "ast.h"
#include "lut.h"

void track_scope_root(ast_node node);

void analyze_semantics(lut_table symbol_table, ast_node root);

#endif // SEMANTIC_H