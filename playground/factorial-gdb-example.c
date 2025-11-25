#include <stdio.h>

int factorial(int n);

/*
 * This example is an exercise on how to use GDB.
 * There's a problem in this example and we can use GDB to
 * find out where is the problem.
 *
 * Reference:
 * https://cgi.cse.unsw.edu.au/~learn/debugging/modules/gdb_basic_use/
 */
int main(void)
{
	int n, f;

	n = 5;
	f = factorial(n);
	printf(
		"The factorial of %d is %d\n",
		n,
		f
	);

	n = 17;
	f = factorial(n);
	printf(
		"The factorial of %d is %d\n",
		n,
		f
	);

	return 0;
}

/*
 * Example:
 * 5! = 5 * 4 * 3 * 2 * 1 = 120
 */
int factorial(int n)
{
	int f = 1, i = 1;

	while (i <= n) {
		f = f * i;
		i++;
	}

	return f;
}
