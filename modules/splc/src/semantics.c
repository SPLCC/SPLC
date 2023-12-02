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
        lut_insert(SPLC_TRANS_UNIT_ENV_TOP(current_trans_unit), (const char *)node->val, SPLE_TYPEDEF, SPLE_NULL, NULL,
                   node, node->location);
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
            lut_insert(SPLC_TRANS_UNIT_ENV_TOP(current_trans_unit), (const char *)node->children[i]->val, SPLE_TYPEDEF,
                       SPLE_NULL, NULL, node, node->location);
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
        splc_push_symtable(tunit, 1);

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
        printf("struct: %s %d\n", struct_union_name, tmp_decl_entry_type);
        lut_insert(tunit->envs[(tunit->nenvs) - 1], struct_union_name, tmp_decl_entry_type, SPLE_NULL, NULL, node,
                   node->location);
        lut_insert(tunit->envs[(tunit->nenvs) - 2], struct_union_name, tmp_decl_entry_type, SPLE_NULL, NULL, node,
                   node->location);
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
            SPLC_FERROR(SPLM_SEM_ERR_3, node->location, "redefinition of variable `%s`", var_name);
        }
        else
        {
            printf("variable: %s %d %d %s\n", var_name, decl_entry_type, decl_extra_type, decl_spec_type);
            lut_insert(tunit->envs[(tunit->nenvs) - 1], var_name, decl_entry_type, decl_extra_type, decl_spec_type,
                       node, node->location);
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
                       node, node->location);
            lut_insert(tunit->envs[0], func_name, decl_entry_type, decl_extra_type, decl_spec_type, node,
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
            SPLC_FERROR(SPLM_SEM_ERR_1, node->location, "variable `%s` is undefined", var_name);
        }

        else
        {
            // check unvalid use of indexing operator
            // TODO: check the tree structrue of nodes and figure out the exact fault
            //(but the array structure is diffrent in declaration and usage)
            ast_node var_node = var_entry->root;
            if (var_node->num_child == 1 && fa_node->num_child > 1 && (fa_node->children[1])->type == SPLT_LSB)
            {
                SPLC_FERROR(SPLM_ERR_UNIV, node->location,
                            "Error type 10: applying indexing operator on non-array type variable %s\n", var_name);
            }
            // check unvalide use of Dot
            if ((var_entry->type != SPLE_VAR ||
                 (var_entry->extra_type != SPLE_STRUCT_DEC && var_entry->extra_type != SPLE_UNION_DEC)) &&
                fa_node->num_child > 1 && (fa_node->children[1])->type == SPLT_DOT)
            {
                SPLC_FERROR(SPLM_ERR_UNIV, node->location,
                            "Error type 13: accessing members of a non-structure variable %s\n", var_name);
            }
            else if (var_entry->type == SPLE_VAR &&
                     (var_entry->extra_type == SPLE_STRUCT_DEC || var_entry->extra_type == SPLE_UNION_DEC))
            {
                const char *struct_union_name = var_entry->spec_type;
                int struct_union_type_exists = 0;
                for (int i = 0; i < tunit->nenvs; i++)
                {
                    if (lut_exists(tunit->envs[i], struct_union_name, SPLE_UNION_DEC) ||
                        lut_exists(tunit->envs[i], struct_union_name, SPLE_STRUCT_DEC))
                        struct_union_type_exists = 1;
                }
                // first check if the struct type is declared
                if (!struct_union_type_exists)
                {
                    // TODO: move it to the definition of the variable
                    /*
                    SPLC_FERROR(SPLM_ERR_UNIV, node->location, "Error: using an undeclared struct/union type %s\n",
                    struct_union_name);
                    */
                }
            }
        }
    }

    // check errors when using functions
    if (node->type == SPLT_FUNC_INVOC_EXPR) // FuncInvocExpr
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
            SPLC_FERROR(SPLM_ERR_UNIV, node->location, "Error type 2: function %s is undefined\n", func_name);
        }
        else if (!func_is_defined && func_name_is_defined)
        {
            SPLC_FERROR(SPLM_ERR_UNIV, node->location,
                        "Error type 11: applying function invocation operator on name %s\n", func_name);
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

        if (node->type == SPLT_FUNC_INVOC_EXPR && child->type == SPLT_ID)
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

sem_expr_t sem_ast_expr_process(const ast_node node, splc_trans_unit tunit)
{
    if (node->symtable)
    {
        tunit->envs[tunit->nenvs] = node->symtable;
        tunit->nenvs++;
    }

    if (SPLT_IS_ID(node->type))
    {
        // return the specific type directly
        lut_entry ent = find_envs(tunit, (char *)(node->val), SPLE_VAR);
        if (ent)
        {
            const char *type = ent->spec_type;
            if (type == splc_token2str(SPLT_TYPE_INT))
                return EXPR_INT;
            else if (type == splc_token2str(SPLT_TYPE_FLOAT))
                return EXPR_FLOAT;
            else if (type == splc_token2str(SPLT_TYPE_CHAR))
                return EXPR_CHAR;
        }
        return EXPR_NULL;
    }
    else if (SPLT_IS_LITERAL(node->type))
    {
        splc_token_t type = node->children[0]->type;
        if (type == SPLT_LTR_INT)
            return EXPR_INT;
        else if (type == SPLT_LTR_FLOAT)
            return EXPR_FLOAT;
        else if (type == SPLT_LTR_CHAR)
            return EXPR_CHAR;
        return EXPR_NULL;
    }
    
    // FuncInvocExpr
    if (node->type == SPLT_FUNC_INVOC_EXPR)
    {
        if (node->num_child == 1) {
            return sem_ast_expr_process(node->children[0], tunit);
        }
        else if (node->num_child == 2)
        {
            ast_node id_node = node->children[0];
            ast_node arg_list_node = node->children[1];

            sem_expr_t func_type = sem_ast_expr_process(id_node, tunit);

            // 如何比较参数是否合法？
            for (int i = 0; i < arg_list_node->num_child; i++) {
                sem_ast_expr_process(arg_list_node->children[i], tunit);
            }

        }

        

        // if (node->num_child == 2)
        // {
        //     // ArgList, each child is Expr
        //     ast_node arg_list_node = node->children[1];
        //     // check the length of arguments
        //     ast_node param_list_node = ent->root->children[1]->children[0];
            

        //     for (int i = 0; i < arg_list_node->num_child; i++) {
                
        //     }
            
        // }

        return EXPR_NULL;
    }

    if (node->type == SPLT_EXPR)
    {
        if (node->num_child == 1)
        {
            // 带有括号的expr、字面量
            return sem_ast_expr_process(node->children[0], tunit);
        }
        else if (node->num_child == 2)
        {
            // unary
            int expr_idx = node->children[0]->type == SPLT_ID || node->children[0]->type == SPLT_EXPR ? 0 : 1;
            ast_node expr_node = node->children[expr_idx];
            ast_node operand_node = node->children[1 - expr_idx];

            sem_expr_t type = sem_ast_expr_process(expr_node, tunit);
            if (type == EXPR_NULL)
            {
                SPLC_ERROR(SPLM_SEM_ERR_7, node->location, "unmatching operands");
            }
            return type;
        }
        else if (node->num_child == 3)
        {
            // TODO Dot
            if (node->type == SPLT_DOT)
            {
                
            }

            sem_expr_t left = sem_ast_expr_process(node->children[0], tunit);
            sem_expr_t right = sem_ast_expr_process(node->children[2], tunit);
            if (left == SPLT_NULL || right == SPLT_NULL || left != right)
            {
                if (node->children[1]->type == SPLT_ASSIGN)
                    SPLC_ERROR(SPLM_SEM_ERR_5, node->location, "unmatching types");
                else
                    SPLC_ERROR(SPLM_SEM_ERR_7, node->location, "unmatching operands");
            }
        }
    }

    for (int i = 0; i < node->num_child; i++)
    {
        sem_ast_expr_process(node->children[i], tunit);
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
    sem_ast_expr_process(tunit->root, tunit);
}

// struct.a