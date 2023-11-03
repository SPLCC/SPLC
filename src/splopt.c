#include "splopt.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* From `spldef.h` */

int spl_include_dir_cnt = 0;
const char **spl_include_dirs = NULL;

int spl_src_file_cnt = 0;
const char **spl_src_files = NULL;

/* From `splopt.h` */

int spl_opterror = 1;
int spl_optind = 1;
char spl_optopt = '\0';
char *spl_optarg = NULL;

int spl_getopt(int nargc, char *nargv[], const char *ostr)
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

void spl_process_args(int nargc, char *nargv[])
{
    int opcode;
    while ((opcode = spl_getopt(nargc, nargv, "I:")) != -1)
    {
        switch (opcode)
        {
        case 0: {
            /* Save file names */
            ++spl_src_file_cnt;
            const char **new_filev = (const char **)realloc(spl_src_files, spl_src_file_cnt * sizeof(char *));
            if (new_filev == NULL)
            {
                fprintf(stderr, "Failed to reallocate source file array: %s\n", nargv[spl_optind]);
                exit(1);
            }
            spl_src_files = new_filev;
            spl_src_files[spl_src_file_cnt - 1] = spl_optarg;
            break;
        }
        case 1: {
            switch (spl_optopt)
            {
            case 'I':
                /* Save include directories */
                ++spl_include_dir_cnt;
                const char **new_filev = (const char **)realloc(spl_include_dirs, spl_include_dir_cnt * sizeof(char *));
                if (new_filev == NULL)
                {
                    fprintf(stderr, "Failed to reallocate include file array: %s\n", nargv[spl_optind]);
                    exit(1);
                }

                char *target = spl_optarg;
                size_t dirlen = strlen(spl_optarg);
                if (dirlen > 0 && spl_optarg[dirlen - 1] != SPL_SYS_DIR_SEPARATOR)
                {
                    target = (char *)malloc((dirlen + 1) * sizeof(char));
                    if (target == NULL)
                    {
                        fprintf(stderr, "Failed to allocate new file name: %s\n", nargv[spl_optind]);
                        exit(1);
                    }
                    memcpy(target, spl_optarg, dirlen + 1);
                    target[dirlen] = SPL_SYS_DIR_SEPARATOR;
                    target[dirlen + 1] = '\0';
                }
                spl_include_dirs = new_filev;
                spl_include_dirs[spl_include_dir_cnt - 1] = target;
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