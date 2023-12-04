#define NULL (void *)0
typedef unsigned long long size_t;
/* From `splcdef.h` */

/* fp test */
float k = 2.3e-5;

int splc_incl_dir_cnt = 0;
const char **splc_incl_dirs = NULL;

int splc_src_file_cnt = 0;
const char **splc_src_files = NULL;

/* From `splcopt.h` */

int splc_opterror = 1;
int splc_optind = 1;
char splc_optopt = '\0';
const char *splc_optfull = NULL;
const char *splc_optarg = NULL;

/* Own definitions */
typedef struct option
{
    int *const target_opt;
    const int opt_abbr;
    const char *opt_name;
} option;

#define OPT_CNT 5
static const option options[OPT_CNT] = {
    {&splcf_verbose, -1, "fverbose"},
    {&splcf_no_diagnostics_color, -1, "fno-diagnostics-color"},
    {&splcf_ast_dump, -1, "ast-dump"},
    {&splcf_enable_ast_punctuators, -1, "fenable-ast-punctuators"},
    {&splcf_no_ast_color, -1, "fno-ast-color"},
};

// clang-format off
void usage()
{
    printf("usage: \033[1m%s\033[0m [options] [file ...]\n%s%s%s%s%s%s%s", progname,
           "  -h                          print this usage and exit\n",
           "  -fverbose                   print all available diagnostic information\n",
           "  -fno-diagnostics-color      do not color diagnostic information\n",
           "  -ast-dump                   dump generated AST to stdout\n",
           "  -fenable-ast-punctuators    append punctuators in AST\n",
           "  -fcolor-ast                 color the output AST\n",
           "  -I<include-directory>       specify extra directory for #include search\n");
}
// clang-format on

int splc_getopt(const int nargc, const char *const nargv[], const char *ostr)
{
    if (splc_optind >= nargc)
        return -1;

    splc_optopt = '\0';
    splc_optfull = NULL;
    splc_optarg = NULL;

    const char *arg = nargv[splc_optind];
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
                SPLC_FMSG_NOLOC(SPLM_ERR_UNIV, "This option requires an argument: %s", nargv[splc_optind]);
            ++splc_optind;
            return SPLC_OPT_BADCH;
        }
    }
    else if (*(arg + 1) != '\0')
    {
        if (splc_opterror)
            SPLC_FMSG_NOLOC(SPLM_ERR_UNIV, "Unrecognized option: %s", nargv[splc_optind]);
        ++splc_optind;
        return SPLC_OPT_BADCH;
    }

    ++splc_optind;
    return 1;
}

static void add_source_file(const char *const file)
{
    ++splc_src_file_cnt;
    const char **new_filev = (const char **)realloc(splc_src_files, splc_src_file_cnt * sizeof(char *));
    SPLC_ALLOC_PTR_CHECK(new_filev, "failed to allocate array for storing source filenames");
    splc_src_files = new_filev;
    const char *filename = strdup(file);
    SPLC_ALLOC_PTR_CHECK(filename, "failed to allocate array for storing source filenames");
    splc_src_files[splc_src_file_cnt - 1] = filename;
}

static void add_include_directory(const char *const dir)
{
    ++splc_incl_dir_cnt;
    const char **new_filev = (const char **)realloc(splc_incl_dirs, splc_incl_dir_cnt * sizeof(char *));
    SPLC_ALLOC_PTR_CHECK(new_filev, "failed to allocate array for storing include directories");

    char *target = strdup(dir);
    size_t dirlen = strlen(dir);
    if (dirlen > 0 && dir[dirlen - 1] != SYSTEM_PATH_SEPARATOR)
    {
        target = (char *)malloc((dirlen + 2) * sizeof(char));
        SPLC_ALLOC_PTR_CHECK(target, "failed to allocate buffer for include directory name");
        memcpy(target, dir, dirlen);
        target[dirlen] = SYSTEM_PATH_SEPARATOR;
        target[dirlen + 1] = '\0';
    }
    splc_incl_dirs = new_filev;
    splc_incl_dirs[splc_incl_dir_cnt - 1] = target;
}

void splc_process_args(const int nargc, const char *nargv[])
{
    int opcode;
    while ((opcode = splc_getopt(nargc, nargv, "I:h")) != -1)
    {
        switch (opcode)
        {
        case 0: { /* a direct argument is present */
            /* Save file name */
            add_source_file(splc_optarg);
            /* Save the directory of file */
            /* Check if there a slash. */
            int dir_present_flag = 0;
            const char *ptr = splc_optarg;
            ptr += strlen(splc_optarg);
            while (*(ptr) != SYSTEM_PATH_SEPARATOR && ptr >= splc_optarg)
                --ptr;
            if (ptr >= splc_optarg) /* found the path separator */
            {
                size_t len = ptr - splc_optarg + 1;
                char *buffer = malloc(len + 1);
                memcpy(buffer, splc_optarg, len);
                buffer[len] = '\0';
                add_include_directory(buffer);
                free(buffer);
            }
            break;
        }
        case 1: { /* A known abbreviated option is present */
            switch (splc_optopt)
            {
            case 'I': {
                /* Save include directories */
                add_include_directory(splc_optarg);
                break;
            }
            case 'h': {
                usage();
                exit(0);
            }
            case SPLC_OPT_BADCH: {
                break;
            }
            default: {
                int flag = 0;
                for (size_t i = 0; i < OPT_CNT; ++i)
                {
                    if (options[i].opt_abbr != -1 && options[i].opt_abbr == splc_optopt)
                    {
                        *(options[i].target_opt) = 1;
                        flag = 1;
                    }
                }
                if (!flag)
                {
                    usage();
                    if (splc_opterror)
                        SPLC_FMSG_NOLOC(SPLM_ERR_UNIV, "Unsupported option: %c", splc_optopt);
                }
                break;
            }
            }
            break;
        }
        case 2: { /* a full argument is present */
            int flag = 0;
            for (size_t i = 0; i < OPT_CNT; ++i)
            {
                if (strcmp(splc_optfull, options[i].opt_name) == 0)
                {
                    *(options[i].target_opt) = 1;
                    flag = 1;
                }
            }
            if (!flag)
            {
                SPLC_FMSG_NOLOC(SPLM_ERR_UNIV, "unrecognized option: %s", splc_optfull);
            }
            break;
        }
        default:
            break;
        }
    }
}