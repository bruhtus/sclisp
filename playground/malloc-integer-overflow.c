#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <stdint.h>

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
 *
 * The maximum value of size_t is SIZE_MAX from stdint.h,
 * and seems like the value of SIZE_MAX on 64-bit system
 * is the same as the value of ULLONG_MAX (maximum value
 * for unsigned long long type) with the value of
 * ((2 ^ 64) - 1). And that value exceed PTRDIFF_MAX value
 * on 64-bit system which is the same as LLONG_MAX (maximum
 * value for signed long long type) with the value of
 * (((2 ^ 64) / 2) - 1).
 *
 * Seems like malloc() implementation prevent us to
 * allocate more than PTRDIFF_MAX bytes as an object that
 * large could cause later __pointer subtraction__ to
 * overflow.
 *
 * References:
 * - https://splone.com/blog/2015/3/11/integer-overflow-prevention-in-c/
 * - https://pvs-studio.com/en/blog/posts/cpp/a0050/
 */
int main(void)
{
	printf("PTRDIFF_MAX:\t%lld\n", PTRDIFF_MAX);
	printf("SIZE_MAX:\t%llu\n\n", SIZE_MAX);

	unsigned int anu = UINT_MAX;
	size_t *p = malloc(anu);

	printf("malloc not overflow: %p\n", p);
	printf("size not overflow: %u\n", anu);

	p = malloc(anu++);

	printf("malloc overflow: %p\n", p);
	printf("size overflow: %u\n", anu);

	return 0;
}
