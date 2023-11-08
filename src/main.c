#include "ast.h"
#include "lex.yy.h"
#include "lut.h"
#include "semantics.h"
#include "splc_link.h"
#include "splcdef.h"
#include "splcopt.h"
#include "syntax.tab.h"
#include "utils.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/*
    List of macro options:
    - `DEBUG`: allow special debug macros
    - `SPLC_DISABLE_DIAG`: disable all diagnostic information
*/

splc_trans_unit *splc_trans_unit_list = NULL;

splc_trans_unit current_trans_unit = NULL;

const char *progname = "splc";

const char *progversion = "0.0.1 alpha";

int main(int argc, char *argv[])
{
    splc_process_args(argc, argv);

    print_prog_diag_info();

    if (splc_src_file_cnt == 0)
        splcfail("no input file\ncompilation terminated.");

    splc_trans_unit_list = (splc_trans_unit *)malloc(splc_src_file_cnt * sizeof(splc_trans_unit));
    SPLC_ALLOC_PTR_CHECK(splc_trans_unit_list, "out of memory");

    for (int i = 0; i < splc_src_file_cnt; ++i)
    {
        splc_trans_unit_list[i] = NULL;
    }

    for (int i = 0; i < splc_src_file_cnt; ++i)
    {
        SPLC_FDIAG("reading source file: %s", splc_src_files[i]);

        if (splc_enter_root(splc_src_files[i]) != 0)
            SPLC_FEXIT_NOLOC("no such file: %s\ncompilation terminated.", splc_src_files[i]);

        current_trans_unit = splc_create_trans_unit();

        /* Start parsing */
        yyparse();

#ifndef DEBUG
        if (!err_flag)
#endif
            ast_print(current_trans_unit->root);

        if (SPLC_OPT_REQUIRE_AST_PREP)
            ast_preprocess(current_trans_unit->root);

        /* TODO: semantic analysis on AST */
        if (!err_flag)
        {
            sem_analyze(current_trans_unit->symbol_table, current_trans_unit->root);
        }

        /* clear and store translation unit */
        current_trans_unit->err_flag = err_flag;
        splc_trans_unit_list[i] = current_trans_unit;

        err_flag = 0;
    }

    int all_err = 0;
    for (int i = 0; i < splc_src_file_cnt; ++i)
        all_err = all_err || splc_trans_unit_list[i]->err_flag;

    if (all_err)
        return 1;

    return 0;
}