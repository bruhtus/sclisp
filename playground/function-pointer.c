#include <stdio.h>

/*
 * Reference:
 * https://www.geeksforgeeks.org/c/function-pointer-in-c/
 */
int add(int a, int b)
{
	return a + b;
}

int main(void)
{
	int (*fptr)(int, int);

	fptr = add;
	/* fptr = &add; */

	printf("%d\n", fptr(1, 2));
	return 0;
}
