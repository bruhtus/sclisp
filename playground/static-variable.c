#include <stdio.h>

/*
 * This is different from global variable.
 * This is called "file scope", which means when the static
 * keyword appear at "file scope", the variable or function
 * will not be visible outside the file.
 *
 * References:
 * - https://stackoverflow.com/q/2928957
 * - https://software.codidact.com/posts/285050/291540#answer-291540
 * - https://www.codequoi.com/en/local-global-static-variables-in-c
 */
static int s_var;

void my_printf(void);
void my_printf2(void);

int main(void)
{
	static int s_var = 42;
	int i;

	my_printf();

	printf("main: %d\n", s_var);

	my_printf2();

	return 0;
}

void my_printf(void)
{
	static int s_var = 69;
	printf("my_printf: %d\n", s_var);
}

void my_printf2(void)
{
	printf("my_printf2: %d\n", s_var);
}
