#include <stdio.h>
#include "parse_command.h"
#include <stdlib.h>
#include <string.h>
int main(int argc, char const *argv[])
{

	while (1)
	{

		char *line = NULL; // Initialize line pointer to NULL for dynamic allocation
		size_t len = 0;	   // Initialize size to 0
		long read;

		read = getline(&line, &len, stdin);
		// Read a line from standard input
		if (read == -1)
		{
			printf("Error\n");
		}
		else
		{
			char **output = parse_command(line);
			for (int i = 0; output[i] != NULL; i++)
			{
				int len = strlen(output[i]);
				printf("%s %d\n", output[i], len);
			}
			free(output);
		}

		/* code */
	}
	return 0;
}
