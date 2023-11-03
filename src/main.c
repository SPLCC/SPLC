#include "ast.h"
#include "lex.yy.h"
#include "lut.h"
#include "spldef.h"
#include "splopt.h"
#include "syntax.tab.h"
#include "utils.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

ast_node root = NULL;

const char *progname = "splc";

int main(int argc, char *argv[])
{
    spl_process_args(argc, argv);

    //===========
    for (int i = 0; i < spl_src_file_cnt; ++i)
    {
        printf("Diagnostics: source file: %s\n", spl_src_files[i]);
    }
    for (int i = 0; i < spl_include_dir_cnt; ++i)
    {
        printf("Diagnostics: include directories: %s\n", spl_include_dirs[i]);
    }
    //===========

    if (spl_src_file_cnt == 0)
    {
        fprintf(stderr, "%s: \033[31mfatal error:\033[0m no input file\ncompilation terminated.\n", progname);
        exit(1);
    }

    for (int i = 0; i < spl_src_file_cnt; ++i)
    {
        if (spl_enter_root(spl_src_files[i]) != 0)
        {
            fprintf(stderr, "%s: \033[31mfatal error:\033[0m no such file: %s\ncompilation terminated.\n", progname,
                    spl_src_files[i]);
            exit(1);
        }

        /* Start parsing */
        yyparse();

        if (!err_flag)
            print_ast(root);
    }

    if (err_flag)
        return 1;

    return 0;
}