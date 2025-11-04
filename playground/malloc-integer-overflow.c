#include <stdio.h>
#include <stdlib.h>
#include <limits.h>

/*
 * This is an example of integer overflow when we are using malloc().
 *
 * The size that we expect to give to malloc() might not be
 * the right size because of the result from
 * arithmetic operation might exceed the maximum value
 * the type of those operand in the operation.
 *
 * In this example, we are exceeding the maximum value
 * of unsigned integer and because of that, the size that
 * being passed to malloc() is not what we expect.
 */
int main(void)
{
	/*
	 * Reference:
	 * https://splone.com/blog/2015/3/11/integer-overflow-prevention-in-c/
	 */
	unsigned int anu = UINT_MAX;
	size_t *p = malloc(anu);

	printf("malloc not overflow: %p\n", p);
	printf("size not overflow: %u\n", anu);

	p = malloc(anu++);

	printf("malloc overflow: %p\n", p);
	printf("size overflow: %u\n", anu);

	return 0;
}
