#include <stdio.h>
#include <limits.h>

/*
 * To check the size limit of our operating system,
 * we can the manpage like this:
 * man limits.h
 */
int main(void)
{
	/*
	 * This will result in undefined behavior as
	 * we are passing the maximum amount of
	 * signed integer value. For the unsigned integer,
	 * the overflow behavior is defined as modulo
	 * wrapping, like:
	 * UINT_MAX + 1 == 0
	 * UINT_MAX + 2 == 1
	 * UINT_MAX + 3 == 2
	 *
	 * What happen here is that when do some arithmetic
	 * calculation, the result of those calculation need
	 * to be between the minimum and maximum value of the
	 * type. INT_MAX is the maximum value of integer type.
	 *
	 * So, before assigning the result to the variable
	 * on the left side, we need to make sure that
	 * the result of arithmetic calculation on the
	 * right side is not exceeding the minimum or
	 * maximum value of the operand type.
	 *
	 * References:
	 * - https://www.gnu.org/software/c-intro-and-ref/manual/html_node/Integer-Overflow.html
	 */
	int a = INT_MAX;
	long itu = a + 1;
	printf("itu: %ld\n", itu);

	/*
	 * Here's another example using short integer with
	 * printf().
	 *
	 * The example with short integer is different
	 * from previous example because the overflow
	 * happen on the printf() evaluation instead of
	 * b + 1 evaluation.
	 *
	 * Why the overflow not happen on b + 1 evaluation?
	 * Because 1 is an integer type. What happen here
	 * is that we have integral promotion of short integer
	 * to integer. The short integer type of b variable
	 * has change to integer type to match the type
	 * of value 1. So the result of b + 1 is integer
	 * value, and not short integer value.
	 *
	 * The only way i can think of to create an integer
	 * overflow scenario using short integer is using
	 * printf() with short integer format specifier.
	 *
	 * Reference about usual arithmetic conversions:
	 * https://port70.net/~nsz/c/c99/n1256.html#6.3.1.8
	 */
	short b = SHRT_MAX;
	int ini = b + 1;
	printf("ini: %hd\n", ini);

	/*
	 * This is not integer overflow but this is another
	 * example of the result from the arithmetic
	 * calculation is not as expected because both of the
	 * operand type is integer so the result is an integer
	 * too.
	 *
	 * The example below will result in 0.000 instead of
	 * 0.500.
	 *
	 * If we want to make the result to have float type,
	 * we can change one of the operand type to float and
	 * the compiler will do a promotion from integer type
	 * to float type. We can do that like this:
	 * float anu = 1.0f / 2;
	 */
	float anu = 1 / 2;
	printf("anu: %.3f\n", anu);

	return 0;
}
