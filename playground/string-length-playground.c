#include <stdio.h>

/*
 * Reference:
 * https://www.geeksforgeeks.org/c/length-of-string-without-using-the-strlen-function-in-c/
 */
int main(void)
{
	const char *str = "anu";
	const char *last = str;

	while (*last)
		last++;

	printf("length: %d\n", last - str);

	return 0;
}
