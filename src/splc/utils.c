#include "utils.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void splperror(const char *msg)
{
    fprintf(stderr, "splc: \033[31mfatal error:\033[0m %s\n", msg);
    exit(1);
}

/* Concatnate two strings with an single char insertion between them. */
char *concat_with_insertion(const char *restrict head, const char *restrict tail, char insertion)
{
    size_t headlen = strlen(head);
    size_t taillen = strlen(tail);
    char *res = (char *)malloc((headlen + taillen + 2) * sizeof(char));
    strcpy(res, head);
    res[headlen] = insertion;
    res[headlen + 1] = '\0';
    strcat(res, tail);
    return res;
}

char *concat(const char *restrict head, const char *restrict tail)
{
    size_t headlen = strlen(head);
    size_t taillen = strlen(tail);
    char *res = (char *)malloc((headlen + taillen + 1) * sizeof(char));
    strcpy(res, head);
    strcat(res, tail);
    return res;
}
