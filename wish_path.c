#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>
#include "wish_path.h"
#include "wish_cmds.h"

void init_paths(){
	search_paths = malloc(sizeof(char*) * 2);
	search_paths[0] = strdup("/bin");
	search_paths[1] = NULL;
	num_paths = 1;
}

void set_paths(char **new_paths, int count){

	for (int i = 0; i < num_paths; i++){
		free(search_paths[i]);
	}
	free(search_paths);

	num_paths = count;
	search_paths = malloc(sizeof(char*) * (count + 1));
	for (int i = 0; i < count; i++){
		search_paths[i] = strdup(new_paths[i]);
	}
	search_paths[count] = NULL;
}