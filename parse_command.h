#ifndef __PARSE_COMMAND_H
#define __PARSE_COMMAND_H
struct Command {
    char* command;
    char **args;
    char* commandArgsString;
};
char** parse_command(char* line);
#endif