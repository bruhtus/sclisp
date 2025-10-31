#include <stdio.h>
#include <stdlib.h>

int main(void)
{
	/*
	 * Reference:
	 * https://stackoverflow.com/a/61082497
	 */
	size_t *p = malloc(-1);

	printf("%p\n", p);

	return 0;
}
