#include "preprocessor.h"
#include "utils.h"
#include "string.h"

char *get_expanded_macro_content(const char *_content)
{
    size_t needed = strlen(_content + 1);
    size_t actual = 0;
    char *result = malloc(needed);
    SPLC_ALLOC_PTR_CHECK(result, "failed to allocate buffer for macro expansion.");
    const char *ptr2 = _content;
    char *ptr1 = result;
    int skippingSpace = 0;
    while (*ptr2 != '\0')
    {
        if (ptr2[0] == '\\' && ptr2[1] == '\r' && ptr2[2] == '\n')
        {
            *ptr1++ = ' ';
            actual++;
            skippingSpace = 0;
            ptr2 += 2;
        }
        else if (ptr2[0] == '\\' && ptr2[1] == '\n')
        {
            *ptr1++ = ' ';
            actual++;
            skippingSpace = 0;
            ptr2 += 1;
        }
        else if (ptr2[0] == ' ')
        {
            if (!skippingSpace)
            {
                *ptr1++ = *ptr2;
                actual++;
                skippingSpace = 1;
            }
        }
        else
        {
            *ptr1++ = *ptr2;
            actual++;
            skippingSpace = 0;
        }
        ptr2 += 1;
    }
    *ptr1 = '\0';
    char *result1 = realloc(result, actual + 1);
    SPLC_ALLOC_PTR_CHECK(result1, "failed to allocate buffer for macro expansion.");
    return result1;
}