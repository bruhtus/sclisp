#include <stdio.h>

/*
 * References:
 * - https://web.archive.org/web/20130126204606/https://blogs.oracle.com/ksplice/entry/the_ksplice_pointer_challenge
 * - https://www.geeksforgeeks.org/c/pointer-arithmetics-in-c-with-examples/
 * - https://stackoverflow.com/a/394777
 */
int main(void)
{
	int x[5];

	/*
	 * This means that we assign the memory address of
	 * array x which has 5 elements into y.
	 *
	 * If we have different elements, let's say something
	 * like:
	 * int (*y)[6] = &x;
	 *
	 * That will throw an error.
	 */
	int (*y)[5] = &x;

	printf("sizeof int: %d\n", sizeof(int));
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
	 * sizeof(int) to the memory address of the first
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
	 */
	printf("&x+1: %p\n", &x+1);
	printf("&x[5]: %p\n", &x[5]);

	/*
	 * This is the representation of the pointer as
	 * long integer value.
	 */
	printf("&x: %ld\n", &x);
	printf("&x+1: %ld\n", &x+1);
	printf("&x+2: %ld\n", &x+2);
	printf("sizeof x: %d\n", sizeof(x));

	return 0;
}
