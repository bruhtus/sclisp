#include <stdio.h>

/*
 * References:
 * - https://web.archive.org/web/20130126204606/https://blogs.oracle.com/ksplice/entry/the_ksplice_pointer_challenge
 * - https://www.geeksforgeeks.org/c/pointer-arithmetics-in-c-with-examples/
 * - https://stackoverflow.com/a/394777
 * - https://stackoverflow.com/a/2528328
 */
int main(void)
{
	char x[5] = {'A', 'B'};

	/*
	 * This means that we assign the memory address of
	 * array x which has 5 elements into y.
	 *
	 * If we have different elements, let's say something
	 * like:
	 * char (*y)[6] = &x;
	 *
	 * That will throw an error.
	 */
	char (*y)[5] = &x;

	printf("sizeof char: %d\n", sizeof(char));
	putchar('\n');

	/*
	 * This is a memory address to the first element of x,
	 * which can be written like this:
	 * &x[0].
	 */
	printf("x: %p\n", x);
	printf("&x[0]: %p\n", &x[0]);
	putchar('\n');

	/*
	 * x+1 here means that we are trying to add the
	 * sizeof(char) to the memory address of the first
	 * elements of x array, which in this case result in
	 * the second elements of x array (x[1]).
	 */
	printf("x+1: %p\n", x+1);
	printf("&x[0]+1: %p\n", &x[0]+1);
	printf("&x[1]: %p\n", &x[1]);
	putchar('\n');

	int i;
	for (i = 0; i < (sizeof(x) / sizeof(x[0])); i++)
		printf("x[%d]: %p\n", i, &x[i]);

	putchar('\n');

	/*
	 * This is a memory address to the array x with 5
	 * elements, which is different from the first
	 * element of x (&x[0]). Even though the value is the
	 * same, but the interpretation is different.
	 */
	printf("&x: %p\n", &x);

	/*
	 * Basically we are trying to access the next memory
	 * address __after__ the last element's memory address.
	 *
	 * By adding 1 in &x + 1, we are adding by the size of
	 * the array. For example, when we have array of 5
	 * characters, and we do &x + 1, we will have the value
	 * off by 5 characters size (getting the size with
	 * sizeof operator).
	 *
	 * The representation would be like this:
	 * 5 char (&x) => 5 char (&x + 1) => 5 char (&x + 2)
	 *
	 * Let's say the memory address of &x is 001 and we
	 * have 1 byte of array's element, the representation
	 * would be:
	 * 001 -> 005 => 006 -> 011 => 012 -> 017
	 */
	printf("&x+1: %p\n", &x+1);
	printf("&x[5]: %p\n", &x[5]);
	printf("&x+2: %p\n", &x+2);
	printf("&x[10]: %p\n", &x[10]);

	putchar('\n');

	/*
	 * This is the representation of the pointer as
	 * long integer value.
	 */
	printf("&x: %ld\n", &x);
	printf("&x+1: %ld\n", &x+1);
	printf("&x+2: %ld\n", &x+2);
	printf("sizeof x: %d\n", sizeof(x));

	putchar('\n');

	printf("*x: %p\n", *x);
	printf("*(&x): %p\n", *(&x));
	printf("**(&x): %p\n", **(&x));

	return 0;
}
