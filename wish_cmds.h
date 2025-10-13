#ifndef WISH_CMDS_H
#define WISH_CMDS_H

extern char **search_paths;
extern int num_paths;

int handle_builtin(char *command, char **args);
void print_error();

#endif