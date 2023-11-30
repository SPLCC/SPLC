#include "ast.h"
#include "lex.yy.h"
#include "lut.h"
#include "semantics.h"
#include "splcdef.h"
#include "splclink.h"
#include "splcopt.h"
#include "splcpass.h"
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
    splc_process_args(argc, (const char **)argv);

    print_prog_diag_info();

    if (splc_src_file_cnt == 0)
    {
        splcerror_noloc(SPLC_ERR_FATAL, "no input file\ncompilation terminated.");
        exit(1);
    }

    splc_trans_unit_list = (splc_trans_unit *)malloc(splc_src_file_cnt * sizeof(splc_trans_unit));
    SPLC_ALLOC_PTR_CHECK(splc_trans_unit_list, "out of memory");

    for (int i = 0; i < splc_src_file_cnt; ++i)
    {
        splc_trans_unit_list[i] = NULL;
    }

    splc_single_pass single_passes[] = {
        &sem_analyze, /* TODO: semantic analysis on AST */
        NULL,
    };

    for (int i = 0; i < splc_src_file_cnt; ++i)
    {
        SPLC_FDIAG("reading source file: %s", splc_src_files[i]);

        if (splc_enter_root(splc_src_files[i]) != 0)
            SPLC_FEXIT_NOLOC("no such file: %s\ncompilation terminated.", splc_src_files[i]);

        current_trans_unit = splc_create_trans_unit();

        /* Start parsing */
        yyparse();

        /* append the global symbol table to AST's root */
        current_trans_unit->root->symtable = lut_copy_table(SPLC_TRANS_UNIT_ENV_TOP(current_trans_unit));

        if (!err_count)
        {
            if (SPLC_OPT_REQUIRE_AST_PREP)
            {
                printf("Generating parsed tree...\n");
                ast_print(current_trans_unit->root);
                printf("Parsed tree generated.\n");
                ast_preprocess(current_trans_unit->root);
            }

            // SPLC_ASSERT(!SPLC_OPT_REQUIRE_AST_PREP); // Preprocessing is done.

            splc_run_single_passes(single_passes, current_trans_unit);
        }

        /* clear and store translation unit */
        current_trans_unit->err_count = err_count;
        current_trans_unit->warn_count = warn_count;
        splc_trans_unit_list[i] = current_trans_unit;
        err_count = 0;
        warn_count = 0;

        if (splcf_ast_dump)
            ast_print(current_trans_unit->root);

        /* output error information */
        if (current_trans_unit->warn_count || current_trans_unit->err_count)
            printf("%d warnings and %d errors generated.\n", current_trans_unit->warn_count,
                   current_trans_unit->err_count);
    }

    int all_err = 0;
    for (int i = 0; i < splc_src_file_cnt; ++i)
        all_err += splc_trans_unit_list[i]->err_count;

    if (all_err)
        return 1;

    return 0;
}