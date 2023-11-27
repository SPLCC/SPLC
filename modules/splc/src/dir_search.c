#include "dir_search.h"
#include "splcdef.h"
#include "utils.h"

#include <errno.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <unistd.h>

static int file_exists(const char *const fullpath)
{
    struct stat buffer;
    return (stat(fullpath, &buffer) == 0 && S_ISREG(buffer.st_mode));
}

char *splc_search_single_path(const char *const _path, const char *const filename)
{
    char *result = NULL;
    char *fullpath = (char *)malloc((strlen(_path) + strlen(filename) + 1) * sizeof(char));
    strcpy(fullpath, _path);
    strcat(fullpath, filename);
    if (file_exists(fullpath))
        result = strdup(fullpath);
    return result;
}

char *splc_search_incl_dirs(const char *const filename)
{
    char *result = NULL;

    /* First check if the file is in current directory */
    if (file_exists(filename))
    {
        result = strdup(filename);
        return result;
    }

    for (int i = 0; i < splc_incl_dir_cnt; ++i)
    {
        if ((result = splc_search_single_path(splc_incl_dirs[i], filename)) != NULL)
        {
            break;
        }
    }
    return result;
}