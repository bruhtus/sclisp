#include <stdio.h>

/*
 * Since every assignment is more than the maximum value
 * of the variable type, the value stored in the variable is
 * truncated to fit the type of the variable.
 *
 * Reference:
 * - https://phrack.org/issues/60/10
 */
int main(void)
{
	int i;
	short s;
	char c;

	i = 0xdeadbeef;
	s = i;
	c = i;

	printf(
		"i = %d (0x%x, %d bits)\n",
		i,
		i,
		sizeof(i) * 8
	);

	printf(
		"s = %d (0x%x, %d bits)\n",
		s,
		s,
		sizeof(s) * 8
	);

	printf(
		"c = %d (0x%x, %d bits)\n",
		c,
		c,
		sizeof(c) * 8
	);

	printf(
		"integral promotion without truncation: %ld\n",
		0xdeadbeef + 1
	);

	/*
	 * Did you notice the difference between this and
	 * the above statement?
	 *
	 * The statement below use format specifier for
	 * integer (%d) instead of long integer (%ld) and
	 * because of that, after the calculation of
	 * 0xdeadbeef + 1 and get the result, which is more
	 * than maximum number of integer, the result get
	 * truncated similar to the assignment operation
	 * previously.
	 *
	 * Basically after getting the result of
	 * 0xdeadbeef + 1, the result value being adjusted
	 * to the target type.
	 *
	 * And because we are using different type in the
	 * calculation, that is long integer + integer, we
	 * are promoting integer to long integer and the
	 * actual result has long integer type.
	 */
	printf(
		"integral promotion with truncation: %d\n",
		0xdeadbeef + 1
	);

	return 0;
}
