#include <stdio.h>

#define SWAP(a, b) {\
	int temp = a;\
	a = b;\
	b = temp;\
\
	printf("address a MACRO: %p\n", &a);\
	printf("address b MACRO: %p\n", &b);\
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
 * https://en.cppreference.com/w/c/language/statements.html#Compound_statements
 */
int main(void)
{
	int a = 69, b = 42;
	printf("address a: %p\n", &a);
	printf("address b: %p\n", &b);

	printf("a: %d, b: %d\n", a, b);

	swap(a, b);
	printf("a: %d, b: %d\n", a, b);

	SWAP(a, b);
	printf("a: %d, b: %d\n", a, b);

	/*
	 * All the code below is example of dangling pointer,
	 * which means that the memory address might or might
	 * not exist after exiting the compound statement
	 * block scope. This is because compound statement has
	 * automatic storage duration.
	 *
	 * When using gcc compiler on my machine,
	 * all these pointer still exist after exiting
	 * the compound statement either because
	 * of gcc implementation or because it is a simple
	 * program that the value in those memory address
	 * still have not replaced yet.
	 *
	 * This usage is unpredictable so we should avoid it.
	 *
	 * In gcc, we can add flag -Wdangling-pointer to give
	 * use warning about dangling pointer usage.
	 *
	 * Reference:
	 * https://stackoverflow.com/q/57775336
	 */
	int *p;

	{
		int i = 69420;
		printf("i address: %p\n", &i);

		p = &i;
	}

	printf("p pointer: %p\n", p);
	printf("p dereference: %d\n", *p);

	for (int i = 0; i < 2; i++) {
		printf("i address: %p\n", &i);
		p = &i;
	}

	printf("p pointer for-loop: %p\n", p);
	printf("p dereference for-loop: %d\n", *p);

	return 0;
}

void swap(int a, int b)
{
	printf("address a in swap(): %p\n", &a);
	printf("address b in swap(): %p\n", &b);

	int temp = a;
	a = b;
	b = temp;
}
