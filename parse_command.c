#include <stdio.h>
#include <string.h>
#include <stdlib.h>
int arg_parse(char* line, int i, int lastSpace)
{
    int len = i - (lastSpace + 1);
    if (len > 1)
    {
        char* command = malloc(sizeof(char) * (len + 1));
        strncpy(command, line + (lastSpace + 1), len);
        command[len] = '\0';
        printf("%s %d\n",command,len);
    }
}
int parse_command(char *line)
{
    int lastSpace = -1;
    int i = 0;
    for (;line[i] != '\0'; i++)
    {
        char currentChar = line[i];
        switch (currentChar)
        {
        case ' ':
        {
            arg_parse(line, i, lastSpace);
            lastSpace = i;
        }
        }
    }
    arg_parse(line, i-1, lastSpace);

}

