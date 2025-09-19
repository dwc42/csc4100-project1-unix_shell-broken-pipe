#include <stdio.h>
int main(int argc, char const *argv[])
{

	char buffer[256];
	while (1)
	{

		char *result = fgets(buffer, sizeof(buffer), stdin);
		if (result == NULL)
		{
			printf("Error\n");
		}
		else
		{
			printf("You entered: %s", buffer);
		}

		/* code */
	}
	return 0;
}
