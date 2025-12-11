#include <stdio.h>

int main(void)
{
	/*
	 * The sizeof operator is evaluated during compile
	 * time, which happen before we run the program.
	 *
	 * That's why when we run the program and print out
	 * the value of x, the value of x did not change.
	 */
	int x = 2;
	printf("sizeof(x++) = %d\n", sizeof(x++));
	printf("sizeof(++x) = %d\n", sizeof(++x));
	printf("x = %d\n", x);

	return 0;
}
