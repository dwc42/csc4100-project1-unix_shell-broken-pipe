#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>
#include "wish_exec.h"
#include "parse_command.h"
#include "wish_path.h"
#include "wish_cmds.h"

char **search_paths = NULL;
int num_paths = 0;

int main(int argc, char const *argv[])
{

	if (argc > 2){
		print_error();
		exit(1);
	}


	init_paths();

	if (argc == 1)
	{
		while (1)
		{

			char *line = NULL; // Initialize line pointer to NULL for dynamic allocation
			size_t len = 0;	   // Initialize size to 0
			long read;
			printf("wish> ");
			read = getline(&line, &len, stdin);
			// Read a line from standard input
			if (read == -1)
			{
				free(line);
				exit(0);
			}
			else
			{
				struct Command *output = parse_command(line);

				int cmd_count = 0;
				while (output[cmd_count].command != NULL){
					cmd_count++;
				}

				if (cmd_count == 1){
					if (!handle_builtin(output[0].command, output[0].args)){
						execute_command(output[0].command, output[0].args, search_paths, output[0].output_file);
					}
				}

				else if (cmd_count > 1)
				{
					pid_t pids[cmd_count];
					int pid_count = 0;
					
					for (int i = 0; i < cmd_count; i++) {

						if (handle_builtin(output[i].command, output[i].args)){
							continue;
						}

						pid_t pid = fork();
						if (pid < 0){
							print_error();
							continue;
						}

						if (pid == 0){

							execute_command_child(output[i].command, output[i].args, search_paths, output[i].output_file);
							
							exit(1);
						}else {
							pids[pid_count++] = pid;
						}

					}
					for (int i = 0; pid_count; i++){
						waitpid(pids[i], NULL, 0);
					}

				}
				
				free(line);
			}

			/* code */
		}
		return 0;
	}

	FILE *fptr;
	fptr = fopen(argv[1], "r");
	if (fptr == NULL)
	{
		print_error();
		exit(1);
	}
	char *line = NULL; // Pointer to the buffer that will hold the line
	size_t len = 0;	   // Size of the buffer
	size_t read;
	while ((read = getline(&line, &len, fptr)) != -1)
	{
		// 'line' now contains the current line, including the newline character if present
		// 'read' contains the length of the line, excluding the null terminator
	struct Command *output = parse_command(line);

	// Count how many commands we have
	int cmd_count = 0;
	while (output[cmd_count].command != NULL) {
    	cmd_count++;
	}

	// Handle single command
	if (cmd_count == 1) {
    	if (!handle_builtin(output[0].command, output[0].args)) {
        	execute_command(output[0].command, output[0].args, search_paths, output[0].output_file);
    	}
	}
	// Handle parallel commands
	else if (cmd_count > 1) {
    	pid_t pids[cmd_count];
    	int pid_count = 0;
    
    	for (int i = 0; i < cmd_count; i++) {
    		if (handle_builtin(output[i].command, output[i].args)) {
            	continue;
        	}
        
        		pid_t pid = fork();
        	if (pid < 0) {
            	print_error();
            	continue;
        	}
        
        	if (pid == 0) {
            	execute_command_child(output[i].command, output[i].args, 
                                search_paths, output[i].output_file);
            	exit(1);
        	} else {
            	pids[pid_count++] = pid;
        }
    }
    
    for (int i = 0; i < pid_count; i++) {
        waitpid(pids[i], NULL, 0);
    }
}
		// You can process the 'line' here
	}
	free(line);
	fclose(fptr); // Close the file
	return 0;
}