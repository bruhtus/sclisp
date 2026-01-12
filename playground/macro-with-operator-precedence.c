#include <stdio.h>

#define MUL_WITHOUT_BRACKET(x) x * x
#define MUL_WITH_BRACKET(x) (x) * (x)

/*
 * References:
 * - https://stackoverflow.com/a/10820434
 * - https://en.cppreference.com/w/c/language/operator_precedence.html
 * - https://stackoverflow.com/a/44770301
 * - https://stackoverflow.com/a/44773955
 */
int main(void)
{
	int a, b, c = 3;

	/*
	 * This will become a = c + 5 * c + 5;
	 * and because operator * has higher precedence than
	 * operator +, the expression become something like
	 * a = c + (5 * c) + 5;
	 */
	a = MUL_WITHOUT_BRACKET(c + 5);
	printf("a1: %d\n", a);

	a = c + (5 * c) + 5;
	printf("a2: %d\n", a);

	/*
	 * This will become b = (c + 5) * (c + 5);
	 * and because we use bracket, the one inside the
	 * bracket will have the higher precedence.
	 */
	b = MUL_WITH_BRACKET(c + 5);
	printf("b1: %d\n", b);

	b = (c + 5) * (c + 5);
	printf("b2: %d\n", b);

	return 0;
}
