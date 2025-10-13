#ifndef WISH_EXEC_H
#define WISH_EXEC_H

void execute_command(char *command, char **args, char **paths, char *output_file);

void execute_command_child(char *command, char **args, char **paths, char *output_file);

#endif