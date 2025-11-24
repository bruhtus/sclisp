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
 * Trying out "compound statement" to swap integer value.
 *
 * The compound statement allows a set of declarations and
 * statements to be grouped into one unit that can be used
 * anywhere a __single statement__ is expected.
 *
 * This is similar to using curly bracket with if statement
 * or for-loop statement.
 *
 * Reference:
 * - https://en.cppreference.com/w/c/language/statements.html#Compound_statements
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
