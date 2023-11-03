#include "ast.h"
#include "lex.yy.h"
#include "lut.h"
#include "spldef.h"
#include "syntax.tab.h"
#include "utils.h"
#include "splopt.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

ast_node root;

const char *progname = "splc";

// static void splperror(const char *msg)
// {
//     fprintf(stderr, "splc: \033[31mfatal error:\033[0m %s\n", msg);
//     exit(1);
// }

const char *files = "";

static void test()
{
}

int main(int argc, char *argv[])
{
    //====================
    int test;
    while ((test = spl_parse_opt(argc, argv, "I:")) != -1)
    {
        if (test == 0)
            printf("found arg: %s\n", spl_optarg);
        else if (test == 1)
            printf("found arg: %c, %s\n", spl_optopt, spl_optarg);
    }
    // return 0;
    //====================

    if (spl_enter_root(argv[1]) != 0)
    {
        fprintf(stderr, "%s: \033[31mfatal error:\033[0m no such file: %s\ncompilation terminated.\n", progname,
                argv[1]);
        exit(1);
    }
    return 0;

    /* Start parsing */
    yyparse();

    if (err_flag)
        return 1;

    return 0;
}