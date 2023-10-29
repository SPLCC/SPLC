#ifndef UTILS_H
#define UTILS_H

void splperror(const char *msg);

/* Concatnate two strings with an single char insertion between them. */
char *concat_with_insertion(const char *restrict head, const char *restrict tail, char insertion);

char *concat(const char *restrict head, const char *restrict tail);

#endif