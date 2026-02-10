#include <stdio.h>

int add(int *restrict, int *restrict);

/*
 * Compile this with flag `-Wall` to get the warning message,
 * like this:
 * gcc -Wall -std=c99 -o playground/restrict-keyword playground/restrict-keyword.c
 *
 * Make sure to use C99 standard or later.
 *
 * Reference:
 * https://www.ralfj.de/blog/2018/07/24/pointers-and-bytes.html
 */
int main(void)
{
	int x;

	/*
	 * With `restrict` keyword, we can't use the same
	 * memory address for multiple arguments. Using
	 * the same memory address for multiple arguments
	 * usually called "aliased pointer".
	 *
	 * Reference:
	 * https://en.wikipedia.org/wiki/Aliasing_(computing)#Aliased_pointers
	 */
	int result = add(&x, &x);
	printf("%d\n", result);

	return 0;
}

int add(int *restrict a, int *restrict b)
{
	*a = 69;

	if (a == b) {
		*b = 42;
	}

	return *a;
}
