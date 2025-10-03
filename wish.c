#include <stdio.h>
#include "parse_command.h"
#include <stdlib.h>
#include <string.h>
int main(int argc, char const *argv[])
{
	if (argc <= 1)
	{
		while (1)
		{

			char *line = NULL; // Initialize line pointer to NULL for dynamic allocation
			size_t len = 0;	   // Initialize size to 0
			long read;
			printf("wish>");
			read = getline(&line, &len, stdin);
			// Read a line from standard input
			if (read == -1)
			{
				printf("Error\n");
			}
			else
			{
				struct Command *output = parse_command(line);
				for (int i = 0; output[i].command != NULL; i++)
				{
					printCommand(output[i]);
				}
			}

			/* code */
		}
		return 0;
	}

	FILE *fptr;
	fptr = fopen(argv[1], "r");
	if (fptr == NULL)
	{
		perror("Error opening file"); // Prints a system-specific error message
		exit(EXIT_FAILURE);
	}
	char *line = NULL; // Pointer to the buffer that will hold the line
	size_t len = 0;	   // Size of the buffer
	size_t read;
	while ((read = getline(&line, &len, fptr)) != -1)
	{
		// 'line' now contains the current line, including the newline character if present
		// 'read' contains the length of the line, excluding the null terminator
		printf("Line read (length %zd): %s\n", read, line);
		struct Command *output = parse_command(line);
		for (int i = 0; output[i].command != NULL; i++)
		{
			printCommand(output[i]);
		}
		// You can process the 'line' here
	}
	free(line);
	fclose(fptr); // Close the file
	return 0;
}
