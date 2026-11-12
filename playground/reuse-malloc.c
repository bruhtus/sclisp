/*
 * This is an example of glibc malloc() reusing the previous heap allocation
 * with the same size.
 *
 * Reference:
 * https://benmandrew.com/articles/what-is-malloc-doing-anyway
 */
#include <stdlib.h>
#include <stdio.h>

int main(void)
{
	unsigned long chunk, mask;

	mask = ~7;
	printf("%lu\n", mask);

	char *p = malloc(1);
	if (!p)
		return 69;

	/*
	 * Because we use `char *`, when using pointer arithmatic,
	 * we only reducing by 1 byte (1 char equal 1 byte).
	 * That's why we need to reduce the pointer by 8 bytes to
	 * get the header info.
	 */
	printf(
		"p - 1 (start chunk): %p, p: %p, p + 31: %p\n",
		(void *)(p - 8),
		p,
		p + 31
	);

	chunk = *((unsigned long *)p - 1);
	printf("chunk p header (check the bits): %lu\n", chunk);

	chunk &= mask;
	printf("chunk p size from header: %lu\n", chunk);

	free(p);

	/*
	 * We are reusing previous allocation here.
	 */
	char *p1 = malloc(24);
	if (!p1)
		return 69;

	printf(
		"p1 - 1 (start chunk): %p, p1: %p, p1 + 31: %p\n",
		(void *)((unsigned long *)p1 - 1),
		p1,
		p1 + 31
	);

	chunk = *((unsigned long *)p1 - 1);
	printf("chunk p1 header (check the bits): %lu\n", chunk);

	chunk &= mask;
	printf("chunk p1 size from header: %lu\n", chunk);

	char *p2 = malloc(24);
	if (!p2)
		return 69;

	printf(
		"p2 - 1 (start chunk): %p, p2: %p, p2 + 31: %p\n",
		(void *)((unsigned long *)p2 - 1),
		p2,
		p2 + 31
	);

	chunk = *((unsigned long *)p2 - 1);
	printf("chunk p2 header (check the bits): %lu\n", chunk);

	chunk &= mask;
	printf("chunk p2 size from header: %lu\n", chunk);

	free(p1);
	free(p2);

	return 0;
}
