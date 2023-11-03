#include "ast.h"
#include "lex.yy.h"
#include "lut.h"
#include "spldef.h"
#include "syntax.tab.h"
#include "utils.h"

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

static void test()
{
}

int main(int argc, char *argv[])
{
    if (argc != 2)
    {
        fprintf(stderr,
                "%s: \033[31mfatal error:\033[0m expected exactly one file to be parsed\ncompilation terminated.\n",
                progname);
        exit(1);
    }

    if (spl_enter_root(argv[1]) != 0)
    {
        fprintf(stderr, "%s: \033[31mfatal error:\033[0m no such file: %s\ncompilation terminated.\n", progname,
                argv[1]);
        exit(1);
    }

    /* Start parsing */
    yyparse();

    if (spl_exit_file() == 0)
    {
        // splerror_nopos(SPLC_ERR_CRIT, "on file inclusion: recursion error");
        exit(1);
    }

    if (err_flag)
        return 1;

    return 0;
}