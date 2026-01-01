#include <stdio.h>

int main(void)
{
	size_t result;

	/*
	 * The overflow checker depends on the `result`
	 * type.
	 *
	 * If the `result` type is `int`,
	 * __builtin_mul_overflow will check if multiplication
	 * result more than INT_MAX value.
	 *
	 * If the `result` type is `size_t`,
	 * __builtin_mul_overflow will check if
	 * multiplication result more than SIZE_MAX.
	 *
	 * Reference:
	 * https://gcc.gnu.org/onlinedocs/gcc/Integer-Overflow-Builtins.html
	 */
	printf(
		"overflow: %d\n",
		__builtin_mul_overflow(
			2305843009213693952,
			8,
			&result
		)
	);

	printf("result: %llu\n", result);

	return 0;
}
