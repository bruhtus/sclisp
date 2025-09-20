#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <editline/readline.h>

/* #define max_input 2048 */

/* static char input[max_input]; */

int main()
{
	puts("Sclisp v0.0.1-rc1");
	puts("Press ctrl-c to exit\n");

	char prompt[128] = "";
	int count = 1;

	sprintf(prompt, "%d-sclisp> ", count);

	while (1) {
		/* fputs("sclisp> ", stdout); */
		/* fgets(input, max_input, stdin); */

		char *input = readline(prompt);

		if (input == NULL || strcmp(input, "exit") == 0) {
			free(input);
			break;
		}

		printf("Holy, C blessing: %s\n", input);
		free(input);
	}

	return 0;
}
