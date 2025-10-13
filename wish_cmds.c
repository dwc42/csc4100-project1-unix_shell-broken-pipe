#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>
#include "wish_cmds.h"

extern char **search_paths;
extern int num_paths;

void print_error(){
	char error_message[30] = "An error has occurred\n";
	write(STDERR_FILENO, error_message, strlen(error_message));
}

int handle_builtin(char *command, char **args){
	if (command == NULL){
		return 0;
	}

	if (strcmp(command, "exit") ==0){
		if (args != NULL && args[0] != NULL){
			print_error();
			return 1;
		}
		exit(0);
	}

	if (strcmp(command, "cd") == 0){
		if (args == NULL || args[0] == NULL || args[1] != NULL){
			print_error();
			return 1;
		}
		if (chdir(args[0]) != 0){
			print_error();
		}
		return 1;
	}

	if (strcmp(command, "path") == 0){
		if (search_paths != NULL){
			for (int i = 0; search_paths[i] != NULL; i++){
				free(search_paths[i]);
			}
			free(search_paths);
		}

		int count = 0;
		if (args != NULL){
			while (args[count] != NULL) count++;
		}

		if (count == 0) {
			search_paths = malloc(sizeof(char*));
			search_paths[0] = NULL;
		} else {
			search_paths = malloc(sizeof(char*) * (count +1));
			for (int i = 0; i < count; i++){
				search_paths[i] = strdup(args[i]);
			}
			search_paths[count] = NULL;
		}
		return 1;
	}

	return 0;
}