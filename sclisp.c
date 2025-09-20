#include <stdio.h>
#include <stdlib.h>

#include <editline/readline.h>

/* #define max_input 2048 */

/* static char input[max_input]; */

int stringcmp(const char *str1, const char *str2);

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

		if (input == NULL || stringcmp(input, "exit") == 0) {
			free(input);
			break;
		}

		printf("Holy, C blessing: %s\n", input);
		free(input);
	}

	return 0;
}

/*
 * References:
 * - https://stackoverflow.com/a/67520047
 * - https://www.reddit.com/r/learnprogramming/comments/7xzaiv/comment/ducbt4y
 *
 * Strings literals are pointers in C.
 * When you see a sentence like "Hello world!",
 * that variable is of the type const char*,
 * meaning it is a pointer to a bunch of chars that cannot be modified.
 */
int stringcmp(const char *str1, const char *str2)
{
	while (*str1 && *str1 == *str2) {
		str1++;
		str2++;
	}

	int result = (unsigned char)*str1 - (unsigned char)*str2;
	return result;
}
