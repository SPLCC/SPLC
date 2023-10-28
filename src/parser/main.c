#include "spldef.h"
#include "ast.h"
#include "lut.h"
#include "utils.h"
#include "parser_syntax.tab.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

ast_node root;

static void usage(const char *progname)
{
    printf("usage: %s [FILE]\n%s", progname, 
           "  FILE           the input file to parse\n");
}

int main(int argc, char *argv[])
{
    if (argc != 2)
    {
        fprintf(stderr, "Invalid number of arguments supported. Expected 1.\n");
        usage(argv[0]);
        exit(1);
    }
    if (freopen(argv[1], "r", stdin) != NULL)
    {
        filename = strdup(argv[1]);
    }
    else
    {
        fprintf(stderr, "spl_parser: \033[31mfatal error:\033[0m %s: No such file\nparsing terminated.\n", argv[1]);
        exit(1);
    }

    /* Start parsing */
    yyparse();

    if (err_flag)
        return 1;
    
    return 0;
}