#include <stdio.h>
#include <assert.h>
#include <limits.h>

void check(unsigned int a);

/*
 * Check core dump generated on arch linux:
 * https://wiki.archlinux.org/title/Core_dump
 */
int main(void)
{
	check(100);
	check(UINT_MAX);
	return 0;
}

void check(unsigned int a)
{
	assert(a + 1 > a);
	printf("%u, %u\n", a + 100, a);
}
