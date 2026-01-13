#include <limits.h>
#include <math.h>
#include <stdio.h>

/*
 * Reference:
 * https://gcc.gnu.org/onlinedocs/gcc/Integer-Overflow-Builtins.html
 */
int main(void)
{
	/*
	 * This is interesting :)
	 */
	unsigned len_mes,
		 len_filename,
		 len_line_number
	;

	unsigned limit,
		 temp_len,
		 carry_out
	;

	len_mes = UINT_MAX - 35;
	len_filename = sizeof(__FILE__);

	/*
	 * If we are using a constant with the math function,
	 * gcc will replace the math operation with the
	 * result. This might not be the case with other
	 * compilers, so just to be safe, always link to the
	 * math library like `-lm`.
	 *
	 * Reference:
	 * https://stackoverflow.com/a/31351654
	 */
	len_line_number = floor(
		log10(__LINE__)
	);

	if (__builtin_uadd_overflow(
		len_filename,
		len_line_number,
		&temp_len
	))
		return 42;

	printf("temp_len: %u\n", temp_len);
	printf("len_mes: %u\n", len_mes);

	limit = __builtin_addc(
		len_mes,
		temp_len,
		4,
		&carry_out
	);

	printf("limit not overflow: %u\n", limit);
	printf("carry_out not overflow: %u\n", carry_out);

	limit = __builtin_addc(
		len_mes,
		temp_len,
		5,
		&carry_out
	);

	printf("limit overflow: %u\n", limit);
	printf("carry_out overflow: %u\n", carry_out);

	return 0;
}
