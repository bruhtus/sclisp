/*
 * This is an example of glibc malloc() might not reusing the previous heap
 * allocation after we freed those allocation.
 *
 * References:
 * - https://benmandrew.com/articles/what-is-malloc-doing-anyway
 * - https://sourceware.org/glibc/wiki/MallocInternals
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
	printf("p - 1: %p, p: %p, p + 31: %p\n", p - 8, p, p + 31);

	chunk = *((unsigned long *)p - 1);
	printf("chunk p header (check the bits): %lu\n", chunk);

	chunk &= mask;
	printf("chunk p size from header: %lu\n", chunk);

	free(p);

	/*
	 * Looks like we didn't reuse the previous allocation that
	 * does not have the same size as the current allocation request,
	 * even though it is already freed (?).
	 *
	 * Maybe these small chunks free list is stored in different category
	 * based on the size? In glibc term, it would be something like "bins" (?).
	 */
	char *p1 = malloc(25);
	if (!p1)
		return 69;

	printf("p1 - 1: %p, p1: %p\n", p1 - 8, p1);

	chunk = *((unsigned long *)p1 - 1);
	printf("chunk p1 header (check the bits): %lu\n", chunk);

	chunk &= mask;
	printf("chunk p1 size from header: %lu\n", chunk);

	char *p2 = malloc(24);
	if (!p2)
		return 69;

	printf("p2 - 1: %p, p2: %p\n", p2 - 8, p2);

	chunk = *((unsigned long *)p2 - 1);
	printf("chunk p2 header (check the bits): %lu\n", chunk);

	chunk &= mask;
	printf("chunk p2 size from header: %lu\n", chunk);

	free(p1);
	free(p2);

	return 0;
}
