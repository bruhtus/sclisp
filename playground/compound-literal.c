#include <stdio.h>

#define SWAP(a, b) {\
	int temp = a;\
	a = b;\
	b = temp;\
\
	printf("pointer a MACRO: %p\n", &a);\
	printf("pointer b MACRO: %p\n", &b);\
}

void swap(int a, int b);

/*
 * Trying out "compound literal" to swap integer value.
 * It seems like we use expression {} or a pair
 * of curly bracket as standalone and still using the
 * same object in the function (?).
 *
 * When we are using compound literal, we are creating
 * unnamed object.
 *
 * References:
 * - https://cs.gmu.edu/~kauffman/cs222/stack-demo.html
 * - https://en.cppreference.com/w/c/language/compound_literal.html
 */
int main(void)
{
	int a = 69, b = 42;
	printf("pointer a: %p\n", &a);
	printf("pointer b: %p\n", &b);

	printf("a: %d, b: %d\n", a, b);

	swap(a, b);
	printf("a: %d, b: %d\n", a, b);

	SWAP(a, b);
	printf("a: %d, b: %d\n", a, b);

	return 0;
}

void swap(int a, int b)
{
	printf("pointer a in swap(): %p\n", &a);
	printf("pointer b in swap(): %p\n", &b);

	int temp = a;
	a = b;
	b = temp;
}
