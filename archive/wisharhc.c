#include <stdio.h>
#include "../parse_command.h"
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>
#include "../wish_exec.h"
int test(char* line) {
    // 'line' now contains the current line, including the newline character if present
		// 'read' contains the length of the line, excluding the null terminator
		struct Command *output = parse_command(line);

		// Count how many commands we have
		int cmd_count = 0;
		while (output[cmd_count].command != NULL)
		{
			cmd_count++;
		}

		// Handle single command
		if (cmd_count == 1)
		{
			if (!handle_builtin(output[0].command, output[0].args))
			{
				execute_command(output[0].command, output[0].args, search_paths, output[0].output_file);
			}
		}
		// Handle parallel commands
		else if (cmd_count > 1)
		{
			pid_t pids[cmd_count];
			int pid_count = 0;

			for (int i = 0; i < cmd_count; i++)
			{
				if (handle_builtin(output[i].command, output[i].args))
				{
					continue;
				}

				pid_t pid = fork();
				if (pid < 0)
				{
					print_error();
					continue;
				}

				if (pid == 0)
				{
					execute_command_child(output[i].command, output[i].args,
										  search_paths, output[i].output_file);
					exit(1);
				}
				else
				{
					pids[pid_count++] = pid;
				}
			}

			for (int i = 0; i < pid_count; i++)
			{
				waitpid(pids[i], NULL, 0);
			}
		}
}