#include "splcopt.h"
#include "splcdef.h"
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
char *splc_optfull = NULL;
char *splc_optarg = NULL;

int splc_getopt(int nargc, char *nargv[], const char *ostr)
{
    if (splc_optind >= nargc)
        return -1;

    splc_optopt = '\0';
    splc_optfull = NULL;
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
    optr = strchr(ostr, splc_optopt);
    if (optr == NULL) /* if this option does not exist */
    {
        /* let the caller determine whether it should be used */
        splc_optfull = arg;
        ++splc_optind;
        return 2;
    }
    if (*(optr + 1) == ':') /* If the option requires an argument */
    {
        splc_optarg = arg + 1;
        if (*splc_optarg == '\0')
        {
            if (splc_opterror)
                SPLC_FWARN_NOLOC("This option requires an argument: %s", nargv[splc_optind]);
            ++splc_optind;
            return SPL_OPT_BADCH;
        }
    }
    else if (*(arg + 1) != '\0')
    {
        if (splc_opterror)
            SPLC_FWARN_NOLOC("Unrecognized option: %s", nargv[splc_optind]);
        ++splc_optind;
        return SPL_OPT_BADCH;
    }

    ++splc_optind;
    return 1;
}

void splc_process_args(int nargc, char *nargv[])
{
    int opcode;
    while ((opcode = splc_getopt(nargc, nargv, "hI:vtp")) != -1)
    {
        switch (opcode)
        {
        case 0: {
            /* Save file names */
            ++splc_src_file_cnt;
            const char **new_filev = (const char **)realloc(splc_src_files, splc_src_file_cnt * sizeof(char *));
            SPLC_ALLOC_PTR_CHECK(new_filev, "failed to allocate array for storing source filenames");
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
                SPLC_ALLOC_PTR_CHECK(new_filev, "failed to allocate array for storing include directories");

                char *target = splc_optarg;
                size_t dirlen = strlen(splc_optarg);
                if (dirlen > 0 && splc_optarg[dirlen - 1] != SPL_SYS_DIR_SEPARATOR)
                {
                    target = (char *)malloc((dirlen + 1) * sizeof(char));
                    SPLC_ALLOC_PTR_CHECK(target, "failed to allocate buffer for include directory name");
                    memcpy(target, splc_optarg, dirlen);
                    target[dirlen] = SPL_SYS_DIR_SEPARATOR;
                    target[dirlen + 1] = '\0';
                }
                splc_incl_dirs = new_filev;
                splc_incl_dirs[splc_incl_dir_cnt - 1] = target;
                break;
            case 'v':
                splc_enable_diag = 1;
                break;
            case 't':
                splc_enable_colored_ast = 1;
                break;
            case 'p':
                splc_enable_ast_punctuators = 1;
                break;
            case 'h':
                usage();
                exit(0);
            default:
                usage();
                if (splc_opterror)
                    SPLC_FWARN_NOLOC("Unsupported option: %c", splc_optopt);
                break;
            }
            break;
        }
        case 2: {
            if (strcmp(splc_optfull, "ast-dump") == 0)
            {
                splc_ast_dump = 1;
            }
            else
            {
                SPLC_FWARN_NOLOC("unrecognized option: %s", splc_optfull);
            }
            break;
        }
        default:
            break;
        }
    }
}

// clang-format off
void usage()
{
    printf("usage: \033[1m%s\033[0m [options] [file ...]\n%s%s%s%s%s%s", progname,
           "  -h                       print this usage and exit\n",
           "  -v                       print diagnostic information\n",
           "  -ast-dump                dump generated AST to stdout\n",
           "  -p                       append punctuators in AST\n",
           "  -t                       color the output AST\n",
           "  -I[{include-directory}]  specify extra directory for #include search\n");
}
// clang-format on

#ifdef DEBUG
#define SPLC_BUILD_DEBUG_VAL 1
#else
#define SPLC_BUILD_DEBUG_VAL 0
#endif

void print_prog_diag_info()
{
    SPLC_FDIAG("%s %s", progname, progversion);
    SPLC_FDIAG("DEBUG=%d", SPLC_BUILD_DEBUG_VAL);
    SPLC_FDIAG("enable_diag=%d", splc_enable_diag);
    SPLC_FDIAG("ast_dump=%d", splc_ast_dump);
    SPLC_FDIAG("enable_ast_punctuators=%d", splc_enable_ast_punctuators);
    SPLC_FDIAG("enable_colored_ast=%d", splc_enable_colored_ast);
    SPLC_FDIAG("opt_require_AST_preprocessing=%d", SPLC_OPT_REQUIRE_AST_PREP);
    for (int i = 0; i < splc_incl_dir_cnt; ++i)
    {
        SPLC_FDIAG("detected include directory: %s", splc_incl_dirs[i]);
    }
}