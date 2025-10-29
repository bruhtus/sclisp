#include <stdio.h>
#include <stdlib.h>

void middleware(int **p);
void release(int **p);

/*
 * Dangling pointer is when an object that is pointed to
 * by a given pointer already deleted or deallocated
 * without modifying the value of that pointer, so
 * the pointer still points to memory location that already
 * deallocated.
 *
 * Once we deallocated the memory, that memory location
 * is available to be used again by malloc() or
 * other allocation function.
 *
 * If we have the original pointer that point to those memory
 * location and try to write to those dangling pointer, we
 * might corrupt unrelated data as the pointer might contain
 * different data.
 *
 * Setting pointer to NULL after deallocated prevent any
 * attempt to read or write to that pointer as we would get
 * segmentation fault, which is generally preferable than
 * random data corruption.
 *
 * This playground to check how to make the pointer null
 * if we passing the pointer to another function.
 *
 * References:
 * - https://stackoverflow.com/a/1879220
 * - https://en.wikipedia.org/wiki/Dangling_pointer
 */
int main(void)
{
	int *p = malloc(sizeof(int));
	printf("init malloc: %p\n", p);

	middleware(&p);
	printf("release malloc: %p\n", p);

	return 0;
}

/*
 * We need to pass pointer of pointer to be able to
 * change the target pointer.
 *
 * If we only use pointer, it will be treated like a value
 * and only have effect __inside__ this function.
 *
 * For example, if we use `void middleware(int *p)` and
 * using release(&p) inside that function, the pointer p will
 * become NULL only in function middleware. After we get out
 * from function middleware, the original pointer p that
 * we passed to function middleware have not changed.
 */
void middleware(int **p)
{
	release(p);
}

/*
 * Reference:
 * https://stackoverflow.com/a/16299414
 */
void release(int **p)
{
	free(*p);
	*p = NULL;
}
