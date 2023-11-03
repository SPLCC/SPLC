#include "splcopt.h"
#include "utils.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* From `splcdef.h` */

int splc_incl_dir_cnt = 0;
const char **splc_incl_dirs = NULL;

int splc_src_file_cnt = 0;
const char **splc_src_files = NULL;

/* From `splcopt.h` */

int splc_opterror = 1;
int splc_optind = 1;
char splc_optopt = '\0';
char *splc_optarg = NULL;

int splc_getopt(int nargc, char *nargv[], const char *ostr)
{
    if (splc_optind >= nargc)
        return -1;

    splc_optopt = '\0';
    splc_optarg = NULL;

    char *arg = nargv[splc_optind];
    char *optr = NULL;

    /* Check if it is an option */
    if (*arg != '-') /* if not, treat it as a direct argument to the program */
    {
        splc_optarg = arg;
        ++splc_optind;
        return 0;
    }
    splc_optopt = *++arg;
    if (splc_optopt == '-') /* no support for '--option' type */
    {
        if (splc_opterror)
            fprintf(stderr, "Unsupported option format: %s\n", nargv[splc_optind]);
        ++splc_optind;
        return SPL_OPT_BADCH;
    }
    optr = strchr(ostr, splc_optopt);
    if (optr == NULL) /* if this option does not exist */
    {
        if (splc_opterror)
            fprintf(stderr, "Unsupported option: %s\n", nargv[splc_optind]);
        ++splc_optind;
        return SPL_OPT_BADCH;
    }
    if (*(optr + 1) == ':') /* If the option requires an argument */
    {
        splc_optarg = arg + 1;
        if (*splc_optarg == '\0')
        {
            if (splc_opterror)
                fprintf(stderr, "This option requires an argument: %s\n", nargv[splc_optind]);
            ++splc_optind;
            return SPL_OPT_BADCH;
        }
    }

    ++splc_optind;
    return 1;
}

void splc_process_args(int nargc, char *nargv[])
{
    int opcode;
    while ((opcode = splc_getopt(nargc, nargv, "I:")) != -1)
    {
        switch (opcode)
        {
        case 0: {
            /* Save file names */
            ++splc_src_file_cnt;
            const char **new_filev = (const char **)realloc(splc_src_files, splc_src_file_cnt * sizeof(char *));
            if (new_filev == NULL)
                SPLC_FFAIL("Failed to reallocate source file array: %s\n", nargv[splc_optind]);
            splc_src_files = new_filev;
            splc_src_files[splc_src_file_cnt - 1] = splc_optarg;
            break;
        }
        case 1: {
            switch (splc_optopt)
            {
            case 'I':
                /* Save include directories */
                ++splc_incl_dir_cnt;
                const char **new_filev = (const char **)realloc(splc_incl_dirs, splc_incl_dir_cnt * sizeof(char *));
                if (new_filev == NULL)
                    SPLC_FFAIL("Failed to reallocate include file array: %s\n", nargv[splc_optind]);

                char *target = splc_optarg;
                size_t dirlen = strlen(splc_optarg);
                if (dirlen > 0 && splc_optarg[dirlen - 1] != SPL_SYS_DIR_SEPARATOR)
                {
                    target = (char *)malloc((dirlen + 1) * sizeof(char));
                    if (target == NULL)
                        SPLC_FFAIL("Failed to allocate new file name: %s\n", nargv[splc_optind]);
                    memcpy(target, splc_optarg, dirlen);
                    target[dirlen] = SPL_SYS_DIR_SEPARATOR;
                    target[dirlen + 1] = '\0';
                }
                splc_incl_dirs = new_filev;
                splc_incl_dirs[splc_incl_dir_cnt - 1] = target;
                break;

            default:
                break;
            }
        }
        break;
        default:
            break;
        }
    }
}