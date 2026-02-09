#include <stdio.h>

#define SIZE 3

int main(void)
{
	char a[SIZE];
	char b[] = {'a', 'b', 'c'};

	int out = snprintf(
		a,
		SIZE,
		"%s def",
		b
	);

	printf("out: %d\n", out);

	/*
	 * If snprintf() return value equal or more than the
	 * provided size, that means the output was truncated.
	 *
	 * If the return value is negative value, it means
	 * there's an error.
	 */
	if (out >= SIZE)
		a[SIZE - 1] = '\0';

	printf("a: %s\n", a);

	return 0;
}
