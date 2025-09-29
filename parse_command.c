#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "libraries/string_to_int.h"
/**
 * https://chatgpt.com/share/68da9304-da00-8004-935b-750989f5bc83
 */
char **split(const char *str, const char *delim)
{
    char *s = strdup(str); // make a modifiable copy
    if (!s)
        return NULL;

    char **result = NULL;
    char *token;
    int count =0;
    while ((token = strsep(&s, delim)) != NULL)
    {
        // grow the array for each token
        result = realloc(result, sizeof(char *) * (count + 1));
        result[count] = strdup(token); // copy token
        count++;
    }
    result[count] = NULL;
    free(s); // strsep moves the pointer, so only the base copy would remain — no need to free
    return result;
}

char** parse_command(char *line)
{
    return split(line, " ");
}
