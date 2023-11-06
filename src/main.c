#include "ast.h"
#include "lex.yy.h"
#include "lut.h"
#include "splcdef.h"
#include "splcopt.h"
#include "syntax.tab.h"
#include "utils.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

ast_node* splc_ast_list = NULL;

ast_node root = NULL;

const char *progname = "splc";

const char *progversion = "0.0.1 alpha";

int main(int argc, char *argv[])
{
    splc_process_args(argc, argv);

    //===========
#ifndef SPLC_DISABLE_DIAG
    print_prog_diag_info();
#endif
    //===========

    if (splc_src_file_cnt == 0)
    {
        splcerror_noloc(SPLC_ERR_CRIT, "no input file\ncompilation terminated.");
        exit(1);
    }

    splc_ast_list = (ast_node *)malloc(splc_src_file_cnt * sizeof(ast_node));
    SPLC_ALLOC_PTR_CHECK(splc_ast_list, "out of memory");

    for (int i = 0; i < splc_src_file_cnt; ++i)
    {
#ifndef SPLC_DISABLE_DIAG
        SPLC_FDIAG("reading source file: %s", splc_src_files[i]);
#endif

        if (splc_enter_root(splc_src_files[i]) != 0)
            SPLC_FEXIT_NOLOC("no such file: %s\ncompilation terminated.", splc_src_files[i]);

        /* Start parsing */
        yyparse();
#ifndef DEBUG
        if (!err_flag)
#endif
            print_ast(root);
    }

    if (err_flag)
        return 1;

    return 0;
}