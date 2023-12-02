#include "semantics.h"
#include "utils.h"

// EXPERIMENTAL
static ast_node find_typedef(const ast_node node)
{
    if (node->type == SPLT_TYPEDEF)
    {
        return node;
    }
    for (int i = 0; i < node->num_child; ++i)
    {
        ast_node tmp = NULL;
        switch (node->children[i]->type)
        {
        case SPLT_DECLTN_SPEC:
        case SPLT_STRG_SPEC:
            if ((tmp = find_typedef(node->children[i])) != NULL)
                return tmp;
            break;
        case SPLT_TYPEDEF:
            return node;
        default:
            break;
        }
    }
    return NULL;
}

// EXPERIMENTAL
static void register_typedef(const ast_node node)
{
    if (node->type == SPLT_ID)
    {
        SPLC_ASSERT(current_trans_unit->nenvs > 0);
        lut_insert(SPLC_TRANS_UNIT_ENV_TOP(current_trans_unit), (const char *)node->val, SPLE_TYPEDEF, SPLE_NULL, NULL, NULL, node, node->location);
    }
    for (int i = 0; i < node->num_child; ++i)
    {
        switch (node->children[i]->type)
        {
        case SPLT_INIT_DEC_LIST:
        case SPLT_INIT_DEC:
        case SPLT_FUNC_DEC:
        case SPLT_DIR_FUNC_DEC:
        case SPLT_DEC:
        case SPLT_DIR_DEC:
            register_typedef(node->children[i]);
            break;
        case SPLT_ID:
            lut_insert(SPLC_TRANS_UNIT_ENV_TOP(current_trans_unit), (const char *)node->children[i]->val, SPLE_TYPEDEF, SPLE_NULL, NULL, NULL, node, node->location);
            break;
        default:
            break;
        }
    }
}

void sem_register_typedef(const ast_node node)
{
    if (node == NULL || find_typedef(node) == NULL)
        return;
    register_typedef(node);
}

int sem_test_typedef_name(const char *name)
{
    lut_entry ent = NULL;
    SPLC_ASSERT(current_trans_unit->nenvs > 0);
    // TODO(sem): fix typedef name scope check
    return (ent = lut_find(SPLC_TRANS_UNIT_ENV_TOP(current_trans_unit), name, SPLE_TYPEDEF)) != NULL &&
           ent->type == SPLE_TYPEDEF;
}

void sem_ast_search(ast_node node, ast_node fa_node, splc_trans_unit tunit, int new_sym_table,
                    splc_entry_t decl_entry_type, splc_entry_t decl_extra_type, const char *decl_spec_type,
                    int in_struct, int in_expr)
{
    // new table construction
    int find_stmt = 0;
    if ((node->type == SPLT_STRUCT_UNION_SPEC && node->num_child == 3) || node->type == SPLT_FUNC_DEF)
        new_sym_table = 1; // Update to avoid pushing a global symbol table when encountering
                           // SPLT_TRANS_UNIT, as there is already one at scope 0.
    if (new_sym_table)
        splc_push_new_symtable(tunit, 1);

    int copy_new_sym_table = new_sym_table;
    if (node->type == SPLT_SEL_STMT || node->type == SPLT_ITER_STMT)
        find_stmt = 1;

    // definition
    // definition of struct/union
    if (node->type == SPLT_STRUCT_UNION_SPEC && node->num_child == 3)
    {
        ast_node type_children = node->children[0];
        ast_node id_children = node->children[1];
        splc_entry_t tmp_decl_entry_type;
        if (type_children->type == SPLT_KWD_STRUCT)
            tmp_decl_entry_type = SPLE_STRUCT_DEC;
        else
        {
            tmp_decl_entry_type = SPLE_UNION_DEC;
        }
        char *struct_union_name = (char *)(id_children->val); // name of struct/union
        // TODO: check if there is a struct(or function) with the same name
        int struct_union_undefined = 0;
        for (int i = 0; i < tunit->nenvs; i++)
        {
            if (lut_exists(tunit->envs[i], struct_union_name, SPLE_STRUCT_DEC))
                struct_union_undefined = 1;
        }
        if (struct_union_undefined)
        {
            SPLC_FERROR(SPLM_ERR_UNIV, node->location, "Error type 15: redefinition of struct/union %s",
                        struct_union_name);
            // printf("Error type 15 at line %d: redefinition of %s\n", node->location.linebegin, struct_union_name);
        }
        printf("struct: %s %d\n",struct_union_name, tmp_decl_entry_type);
        lut_insert(tunit->envs[(tunit->nenvs)-1], struct_union_name, tmp_decl_entry_type, SPLE_NULL, NULL, NULL, node, node->location);
        lut_insert(tunit->envs[(tunit->nenvs)-2], struct_union_name, tmp_decl_entry_type, SPLE_NULL, NULL, NULL, node, node->location);
        in_struct = 1;
    }
    // definition in struct/union
    if (node->type == SPLT_STRUCT_DECLTN) // Struct/Union-Decl
    {
        decl_extra_type = SPLE_NULL;
        ast_node typespec_child_node = ((node->children[0])->children[0])->children[0]; // son of TypeSpec
        if (typespec_child_node->num_child == 0)                                        // variable type
        {
            decl_entry_type = SPLE_VAR;
            decl_spec_type = splc_token2str(typespec_child_node->type);
        }
        else
        { // struct/union type
            // pass two parameter into his brothers for inserting into table
            decl_entry_type = SPLE_VAR;
            ast_node type_children = typespec_child_node->children[0];
            ast_node id_children = typespec_child_node->children[1];
            if (type_children->type == SPLT_KWD_STRUCT)
                decl_extra_type = SPLE_STRUCT_DEC;
            else
            {
                decl_extra_type = SPLE_UNION_DEC;
            }
            decl_spec_type = (char *)(id_children->val);
        }
    }

    // definition of function
    if (node->type == SPLT_FUNC_DEF) // FunctionDef
    {
        decl_entry_type = SPLE_FUNC;
        ast_node typespec_child_node = (((node->children[0])->children[0])->children[0]); // Typespec's child
        if (typespec_child_node->num_child == 0)                                          // variable type
        {
            decl_extra_type = SPLE_VAR;
            decl_spec_type = splc_token2str(typespec_child_node->type);
        }
        else
        { // struct/union type
            ast_node type_children = typespec_child_node->children[0];
            ast_node id_children = typespec_child_node->children[1];
            if (type_children->type == SPLT_KWD_STRUCT)
                decl_extra_type = SPLE_STRUCT_DEC;
            else
            {
                decl_extra_type = SPLE_UNION_DEC;
            }
            decl_spec_type = (char *)(id_children->val);
        }
        // passing the params to the declaration below
    }

    // definition in function
    if (node->type == SPLT_PARAM_DEC || node->type == SPLT_DIR_DECLTN) // ParamDecltr(for each parameter of function) /
                                                                       // DirectDecl(declaration in the functionbody)
    {
        decl_extra_type = SPLE_NULL;
        ast_node typespec_child_node = (((node->children[0])->children[0])->children[0]);
        if (typespec_child_node->num_child == 0) // variable type
        {
            decl_entry_type = SPLE_VAR;
            decl_spec_type = splc_token2str(typespec_child_node->type);
        }
        else
        { // struct/union type
            decl_entry_type = SPLE_VAR;
            ast_node type_children = typespec_child_node->children[0];
            ast_node id_children = typespec_child_node->children[1];
            if (type_children->type == SPLT_KWD_STRUCT)
                decl_extra_type = SPLE_STRUCT_DEC;
            else
            {
                decl_extra_type = SPLE_UNION_DEC;
            }
            decl_spec_type = (char *)(id_children->val);
        }
        // pass the two params to the declaration below
    }

    // add declare variables into table:
    if (node->type == SPLT_DIR_DEC) // DirectDecltr (for variables)
    {
        char *var_name = (char *)((node->children[0])->val);
        // printf("%s %d %d %s\n",var_name, decl_entry_type, decl_extra_type, decl_spec_type);
        //  Type 3 --- redefined variable
        int var_is_redefined = 0;
        if (!in_struct)
        {
            for (int i = 0; i < tunit->nenvs; i++)
            {
                if (lut_exists(tunit->envs[i], var_name, SPLE_VAR))
                    var_is_redefined = 1;
            }
        }
        else
        {
            var_is_redefined = lut_exists(tunit->envs[tunit->nenvs - 1], var_name, SPLE_VAR);
        }
        if (var_is_redefined)
        {
            SPLC_FERROR(SPLM_ERR_SEM_3, node->location, "redefinition of variable `%s`", var_name);
        }
        else
        {
            printf("variable: %s %d %d %s\n", var_name, decl_entry_type, decl_extra_type, decl_spec_type);
            lut_insert(tunit->envs[(tunit->nenvs) - 1], var_name, decl_entry_type,  decl_extra_type, decl_spec_type,
                       NULL, node, node->location);
        }
        return;
    }
    if (node->type == SPLT_DIR_FUNC_DEC) // DirectFunctionDecltr (for functions)
    {
        char *func_name = (char *)(((node->children[0])->children[0])->val);
        int func_is_redefined = 0;
        for (int i = 0; i < tunit->nenvs; i++)
        {
            if (lut_exists(tunit->envs[i], func_name, SPLE_FUNC))
                func_is_redefined = 1;
        }
        if (func_is_redefined)
        {
            SPLC_FERROR(SPLM_ERR_UNIV, node->location, "Error type 4: redefinition of function %s", func_name);
        }
        else
        {
            printf("function: %s %d %d %s\n", func_name, decl_entry_type, decl_extra_type, decl_spec_type);
            lut_insert(tunit->envs[(tunit->nenvs) - 1], func_name, decl_entry_type, decl_extra_type, decl_spec_type,
                       NULL, node, node->location);
            lut_insert(tunit->envs[0], func_name, decl_entry_type, decl_extra_type, decl_spec_type, NULL, node,
                       node->location);
        }
        if (node->num_child == 2)
            sem_ast_search(node->children[1], node, tunit, 0, decl_entry_type, decl_extra_type, decl_spec_type,
                           in_struct, in_expr);
        return;
    }

    // check expr
    if (node->type == SPLT_EXPR)
        in_expr = 1;

    // Type1 --- check variable is undefined
    if (node->type == SPLT_ID && in_expr)
    {
        char *var_name = (char *)(node->val);
        // check whether the variable is defined or not
        int var_is_defined = 0;
        lut_entry var_entry;
        for (int i = 0; i < tunit->nenvs; i++)
        {
            if (lut_exists(tunit->envs[i], var_name, SPLE_VAR))
            {
                var_is_defined = 1;
                var_entry = lut_find(tunit->envs[i], var_name, SPLE_VAR);
            }
        }
        if (!var_is_defined)
        {
            SPLC_FERROR(SPLM_ERR_SEM_1, node->location, "variable `%s` is undefined", var_name);
        }
        else
        {
            // check unvalid use of indexing operator
            // TODO: check the tree structrue of nodes and figure out the exact fault
            //(but the array structure is diffrent in declaration and usage)
            ast_node var_node = var_entry->root;
            int decl_num = (var_node->num_child) / 3;
            int use_num = 0;
            ast_node tmp_node = node->father;
            while (tmp_node->type == SPLT_EXPR && (tmp_node->children[(tmp_node->num_child) - 1])->type == SPLT_RSB)
            {
                use_num++;
                tmp_node = tmp_node->father;
                if ((tmp_node->children[0])->type != SPLT_EXPR)
                    break;
            }
            if (decl_num < use_num)
            {
                SPLC_FERROR(SPLM_ERR_SEM_10, node->location,
                            "applying indexing operation on non-array type variable `%s`", var_name);
            }
        }
    }

    // check errors when using functions
    if (node->type == SPLT_CALL_EXPR) // CallExpr
    {
        char *func_name = ((char *)(node->children[0])->val);
        int func_is_defined = 0;
        for (int i = 0; i < tunit->nenvs; i++)
        {
            if (lut_exists(tunit->envs[i], func_name, SPLE_FUNC))
                func_is_defined = 1;
        }
        int func_name_is_defined = 0;
        for (int i = 0; i < tunit->nenvs; i++)
        {
            if (lut_name_exists(tunit->envs[i], func_name))
                func_name_is_defined = 1;
        }
        if (!func_is_defined && !func_name_is_defined)
        {
            SPLC_FERROR(SPLM_ERR_SEM_2, node->location, "function %s is undefined\n", func_name);
        }
        else if (!func_is_defined && func_name_is_defined)
        {
            SPLC_FERROR(SPLM_ERR_SEM_11, node->location, "applying function invocation operator on name %s\n",
                        func_name);
        }
    }

    // search children
    for (int i = 0; i < node->num_child; i++)
    {
        // get new_sym_table
        ast_node child = node->children[i];
        if (child->type == SPLT_STMT && find_stmt)
        {
            new_sym_table = 1;
            find_stmt = 0;
        }
        else
        {
            new_sym_table = 0;
        }

        // if the child is a Dot node, we will stop variable undefinition checking
        if (child->type == SPLT_DOT)
            in_expr = 0;

        if (node->type == SPLT_CALL_EXPR && child->type == SPLT_ID)
        {
            sem_ast_search(child, node, tunit, new_sym_table, decl_entry_type, decl_extra_type, decl_spec_type,
                           in_struct, 0);
        }
        else
        {
            sem_ast_search(child, node, tunit, new_sym_table, decl_entry_type, decl_extra_type, decl_spec_type,
                           in_struct, in_expr);
        }
    }

    // pop symbol table and link it to the node
    if (copy_new_sym_table)
    {
        lut_table top_sym_table = splc_pop_symtable(tunit);
        node->symtable = top_sym_table;
    }
}

lut_entry find_envs(const splc_trans_unit tunit, const char *name, const splc_entry_t type)
{
    lut_entry ent = NULL;
    for (int i = 0; i < tunit->nenvs; i++)
    {
        ent = lut_find(tunit->envs[i], name, type);
        if (ent)
        {
            return ent;
        }
    }
    return NULL;
}

expr_entry sem_new_expr_entry()
{
    expr_entry ent = (expr_entry)malloc(sizeof(sem_expr_entry_struct));
    SPLC_ALLOC_PTR_CHECK(ent, "out of memory when constructing expr_entry");

    ent->extra_type = SPLE_NULL;
    ent->spec_type = NULL;

    return ent;
}

expr_entry sem_lut2expr(lut_entry ent)
{
    expr_entry result = sem_new_expr_entry();
    result->spec_type = ent->spec_type;
    result->extra_type = ent->extra_type;

    return result;
}

expr_entry sem_process_expr_dot(const expr_entry struct_var_ent, const ast_node node, splc_trans_unit tunit)
{
    // ID1.Dot.ID2
    // check whether exists a struct the spec_type of `ID1` is

    if (struct_var_ent->extra_type == SPLE_STRUCT_DEC)
    {
        lut_entry struct_ent = find_envs(tunit, struct_var_ent->spec_type, SPLE_STRUCT_DEC);
        if (struct_ent)
        {
            lut_entry member_ent = lut_find(struct_ent->root->symtable, (char *)(node->children[2]->val), SPLE_VAR);
            if (member_ent)
            {
                return sem_lut2expr(member_ent);
            }
            else
            {
                // no specific member in the struct declaration
                SPLC_ERROR(SPLM_ERR_SEM_13, node->children[2]->location, "accessing an undefined structure member");
                return NULL;
            }
        }
        else
        {
            // no specific struct declaration
            return NULL;
        }
    }
    else
    {
        SPLC_ERROR(SPLM_ERR_SEM_13, node->children[1]->location, "accessing members of a non-structure variable");
        return NULL;
    }
}

int is_computable(expr_entry ent)
{
    return strcmp(ent->spec_type, splc_token2str(SPLT_TYPE_INT)) == 0 ||
           strcmp(ent->spec_type, splc_token2str(SPLT_TYPE_FLOAT)) == 0 ||
           strcmp(ent->spec_type, splc_token2str(SPLT_TYPE_CHAR)) == 0 ||
           strcmp(ent->spec_type, splc_token2str(SPLT_LTR_INT)) == 0 ||
           strcmp(ent->spec_type, splc_token2str(SPLT_LTR_FLOAT)) == 0 ||
           strcmp(ent->spec_type, splc_token2str(SPLT_LTR_CHAR)) == 0;
}

int are_types_equal(const char *spec_type1, const char *spec_type2, splc_token_t token_type, splc_token_t token_literal)
{
    return (strcmp(spec_type1, splc_token2str(token_type)) == 0 ||
            strcmp(spec_type1, splc_token2str(token_literal)) == 0) &&
           (strcmp(spec_type2, splc_token2str(token_type)) == 0 ||
            strcmp(spec_type2, splc_token2str(token_literal)) == 0);
}

expr_entry sem_process_expr(const ast_node node, splc_trans_unit tunit)
{

    if (node->symtable)
    {
        splc_push_existing_symtable(tunit, node->symtable);
    }

    if (SPLT_IS_ID(node->type))
    {
        // return the specific type directly
        lut_entry ent = find_envs(tunit, (char *)(node->val), SPLE_VAR);
        if (ent)
        {
            return sem_lut2expr(ent);
        }
        return NULL;
    }
    else if (SPLT_IS_LITERAL(node->type))
    {
        splc_token_t type = node->children[0]->type;
        if (type == SPLT_LTR_INT || type == SPLT_LTR_FLOAT || type == SPLT_LTR_CHAR)
        {
            expr_entry ent = sem_new_expr_entry();
            const char *spec_type = splc_token2str(type);
            ent->spec_type = spec_type;
            return ent;
        }
        else
        {
            return NULL;
        }
    }

    // CallExpr
    if (node->type == SPLT_CALL_EXPR)
    {
        // find in global symtable
        return sem_lut2expr(lut_find(tunit->envs[0], (char *)(node->children[0]->val), SPLE_FUNC));
    }

    if (node->type == SPLT_EXPR)
    {
        if (node->num_child == 1)
        {
            // expr or literals with parentheses
            return sem_process_expr(node->children[0], tunit);
        }
        else if (node->num_child == 2)
        {
            // unary
            int expr_idx = node->children[0]->type == SPLT_ID || node->children[0]->type == SPLT_EXPR ? 0 : 1;
            ast_node expr_node = node->children[expr_idx];
            ast_node operand_node = node->children[1 - expr_idx];

            expr_entry ent = sem_process_expr(expr_node, tunit);
            if (!is_computable(ent))
            {
                SPLC_ERROR(SPLM_ERR_SEM_7, node->location, "unmatching operands");
            }
            return ent;
        }
        else if (node->num_child == 3)
        {

            if (node->children[1]->type == SPLT_DOT)
            {
                expr_entry left = sem_process_expr(node->children[0], tunit);
                if (left)
                {
                    return sem_process_expr_dot(left, node, tunit);
                }
                return NULL;
            }
            else
            {
                expr_entry left = sem_process_expr(node->children[0], tunit);
                expr_entry right = sem_process_expr(node->children[2], tunit);

                if (left == NULL || right == NULL)
                {
                    return NULL;
                }

                if (node->children[1]->type == SPLT_ASSIGN)
                {
                    // Literal as left
                    if (left->spec_type == splc_token2str(SPLT_LTR_INT) ||
                        left->spec_type == splc_token2str(SPLT_LTR_FLOAT) ||
                        left->spec_type == splc_token2str(SPLT_LTR_CHAR))
                    {
                        SPLC_ERROR(SPLM_ERR_SEM_6, node->children[0]->location,
                                   "rvalue appears on the left-hand side of the assignment operator");
                        return NULL;
                    }

                    // struct can be assigned but cannot be computed
                    else if (!(left->extra_type == SPLE_STRUCT_DEC && right->extra_type == SPLE_STRUCT_DEC &&
                               left->spec_type == right->spec_type))
                    {
                        SPLC_ERROR(SPLM_ERR_SEM_5, node->location, "unmatching type on both sides of assignment");
                        return NULL;
                    }
                }

                if (!are_types_equal(left->spec_type, right->spec_type, SPLT_TYPE_INT, SPLT_LTR_INT) &&
                    !are_types_equal(left->spec_type, right->spec_type, SPLT_TYPE_FLOAT, SPLT_LTR_FLOAT) &&
                    !are_types_equal(left->spec_type, right->spec_type, SPLT_TYPE_CHAR, SPLT_LTR_CHAR))
                {
                    if (node->children[1]->type == SPLT_ASSIGN)
                    {
                        SPLC_ERROR(SPLM_ERR_SEM_5, node->location, "unmatching type on both sides of assignment ");
                        return NULL;
                    }
                    else
                    {
                        SPLC_ERROR(SPLM_ERR_SEM_7, node->location, "unmatching operand");
                        return NULL;
                    }
                }

                return left;
            }
        }
    }

    for (int i = 0; i < node->num_child; i++)
    {
        sem_process_expr(node->children[i], tunit);
    }

    if (node->symtable)
    {
        splc_pop_symtable(tunit);
    }

    return NULL;
}

void sem_process_func_return(ast_node node, splc_trans_unit tunit)
{
    if (node->symtable)
    {
        splc_push_existing_symtable(tunit, node->symtable);
    }

    if (node->type == SPLT_FUNC_DEF)
    {

        lut_entry func_ent = lut_find(
            node->symtable, (char *)(node->children[1]->children[0]->children[0]->children[0]->val), SPLE_FUNC);

        ast_node jump_stmt_node = ast_find(node, SPLT_JUMP_STMT);

        if ((jump_stmt_node == NULL || jump_stmt_node->num_child == 1) &&
            strcmp(func_ent->spec_type, splc_token2str(SPLT_NULL)) != 0)
        {
            // no return but not void function
            if (jump_stmt_node == NULL)
                SPLC_ERROR(SPLM_ERR_SEM_8, node->children[0]->location, "incompatiable return type");
            else
                SPLC_ERROR(SPLM_ERR_SEM_8, jump_stmt_node->location, "incompatiable return type");
        }
        else
        {
            expr_entry ret_ent = sem_process_expr(jump_stmt_node->children[1], tunit);
            ast_print(jump_stmt_node);
            if (ret_ent)
            {
                printf("%s\n", ret_ent->spec_type);
                if (!are_types_equal(ret_ent->spec_type, func_ent->spec_type, SPLT_TYPE_INT, SPLT_LTR_INT) &&
                    !are_types_equal(ret_ent->spec_type, func_ent->spec_type, SPLT_TYPE_FLOAT, SPLT_LTR_FLOAT) &&
                    !are_types_equal(ret_ent->spec_type, func_ent->spec_type, SPLT_TYPE_CHAR, SPLT_LTR_CHAR) &&
                    !(ret_ent->extra_type == SPLE_STRUCT_DEC && func_ent->extra_type == SPLE_STRUCT_DEC &&
                      (strcmp(ret_ent->spec_type, func_ent->spec_type) == 0)))
                {
                    SPLC_ERROR(SPLM_ERR_SEM_8, jump_stmt_node->location, "incompatiable return type");
                }
            }
        }
        return;
    }

    for (int i = 0; i < node->num_child; i++)
    {
        sem_process_func_return(node->children[i], tunit);
    }

    if (node->symtable)
    {
        splc_pop_symtable(tunit);
    }
}

void sem_analyze(splc_trans_unit tunit)
{
    // TODO(semantics): finish semantic analysis part
    // splcdiag("Semantic Analysis should be performed there.\n");
    sem_ast_search(tunit->root, NULL, tunit, 0, SPLE_NULL, SPLE_NULL, NULL, 0, 0);
    sem_process_expr(tunit->root, tunit);
    sem_process_func_return(tunit->root, tunit);
}

// TODO: 8, 9, 12