#include <stdio.h>

void first(void);
void second(void);

/*
 * This will print out only on the last printf() call, the
 * one with new line character (\n), which is interesting.
 *
 * This make me wonder, even when we use multiple printf(),
 * valgrind still report that we only use 1 memory allocation.
 * Does that mean we use the same memory allocation even
 * though we use multiple printf()?
 */
int main(void)
{
	first();
	second();
	printf("ini: %d\n", 69420);

	return 0;
}

void first(void)
{
	printf("anu: %d", 69);
}

void second(void)
{
	printf("itu: %d", 42);
}
