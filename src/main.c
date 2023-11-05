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

ast_node root = NULL;

const char *progname = "splc";

int main(int argc, char *argv[])
{
    splc_process_args(argc, argv);

    //===========
#ifndef SPLC_DISABLE_DIAG
    for (int i = 0; i < splc_src_file_cnt; ++i)
    {
        SPLC_FDIAG("source file: %s", splc_src_files[i]);
    }
    for (int i = 0; i < splc_incl_dir_cnt; ++i)
    {
        SPLC_FDIAG("include directory: %s", splc_incl_dirs[i]);
    }
#endif
    //===========

    if (splc_src_file_cnt == 0)
    {
        splcerror_noloc(SPLC_ERR_CRIT, "no input file\ncompilation terminated.");
        exit(1);
    }

    for (int i = 0; i < splc_src_file_cnt; ++i)
    {
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