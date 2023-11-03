#ifndef DIR_SEARCH_H
#define DIR_SEARCH_H
/* This file defines utilities for splc to traverse through directories. */
#include <stdio.h>

/* Try to find a file by recursively searching in `path`. The path shall end with the directory separator */
char *splc_search_single_path(const char *const _path, const char *const filename);

/* Try to find a file by searching in `splc_incl_dirs`. This function will not recursively search in subdirectories. */
char *splc_search_incl_dirs(const char *const filename);

#endif