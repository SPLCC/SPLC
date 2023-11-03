#include "splopt.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int spl_opterror = 1;
int spl_optind = 1;
char spl_optopt = '\0';
char *spl_optarg = NULL;

int spl_parse_opt(int nargc, char *nargv[], const char *ostr)
{
    if (spl_optind >= nargc)
        return -1;

    spl_optopt = '\0';
    spl_optarg = NULL;

    char *arg = nargv[spl_optind];
    char *optr = NULL;

    /* Check if it is an option */
    if (*arg != '-') /* if not, treat it as a direct argument to the program */
    {
        spl_optarg = arg;
        ++spl_optind;
        return 0;
    }
    spl_optopt = *++arg;
    if (spl_optopt == '-') /* no support for '--option' type */
    {
        if (spl_opterror)
            fprintf(stderr, "Unsupported option format: %s\n", nargv[spl_optind]);
        ++spl_optind;
        return SPL_OPT_BADCH;
    }
    optr = strchr(ostr, spl_optopt);
    if (optr == NULL) /* if this option does not exist */
    {
        if (spl_opterror)
            fprintf(stderr, "Unsupported option: %s\n", nargv[spl_optind]);
        ++spl_optind;
        return SPL_OPT_BADCH;
    }
    if (*(optr + 1) == ':') /* If the option requires an argument */
    {
        spl_optarg = arg + 1;
        if (*spl_optarg == '\0')
        {
            if (spl_opterror)
                fprintf(stderr, "This option requires an argument: %s\n", nargv[spl_optind]);
            ++spl_optind;
            return SPL_OPT_BADCH;
        }
    }

    ++spl_optind;
    return 1;
}