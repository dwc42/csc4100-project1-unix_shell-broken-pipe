#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "libraries/string_to_int.h"
/**
 * https://chatgpt.com/share/68da9304-da00-8004-935b-750989f5bc83
 */
char **split(const char *str, const char *delim, int *count)
{
    char *s = strdup(str); // make a modifiable copy
    if (!s)
        return NULL;

    char **result = NULL;
    *count = 0;
    char *token;

    while ((token = strsep(&s, delim)) != NULL)
    {
        // grow the array for each token
        result = realloc(result, sizeof(char *) * (*count + 1));
        result[*count] = strdup(token); // copy token
        (*count)++;
    }
    free(s); // strsep moves the pointer, so only the base copy would remain — no need to free
    return result;
}

int parse_command(char *line)
{
    int *count;
    return split(line, " ", count);
}
