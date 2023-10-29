#include "spldef.h"
#include "ast.h"
#include "lut.h"
#include "utils.h"
#include "parser_syntax.tab.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

ast_node root;

// static void splperror(const char *msg)
// {
//     fprintf(stderr, "splc: \033[31mfatal error:\033[0m %s\n", msg);
//     exit(1);
// }

int main(int argc, char *argv[])
{
    if (argc != 2)
    {
        fprintf(stderr, "spl_parser: \033[31mfatal error:\033[0m expected exactly one file to be parsed.\n");
        exit(1);
    }
    if (freopen(argv[1], "r", stdin) != NULL)
    {
        filename = strdup(argv[1]);
    }
    else
    {
        fprintf(stderr, "spl_parser: \033[31mfatal error:\033[0m %s: no such file\nparsing terminated.\n", argv[1]);
        exit(1);
    }

    /* Start parsing */
    yyparse();

    if (err_flag)
        return 1;
    
    return 0;
}