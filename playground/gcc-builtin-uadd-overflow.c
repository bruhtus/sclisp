#include <stdio.h>
#include <limits.h>

int main(void)
{
	unsigned limit;

	unsigned length = UINT_MAX;

	/*
	 * Keep in mind that if the value of first or second
	 * argument has an overflow already, the builtin
	 * function will proceed with the overflow value
	 * instead of the actual value.
	 *
	 * For example, if we do:
	 * __builtin_uadd_overflow(length + 1, 1, &limit)
	 *
	 * the expression above will use the overflow value
	 * of `length + 1`, which is 0 and will add that to
	 * value `1` (the second argument).
	 */
	unsigned overflow = __builtin_uadd_overflow(
		length,
		1,
		&limit
	);

	printf("overflow: %u\n", overflow);

	unsigned not_overflow = __builtin_uadd_overflow(
		length + 1, // Already overflow here.
		1,
		&limit
	);

	printf("not overflow: %u\n", not_overflow);

	return 0;
}
