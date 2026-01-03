#include <stdio.h>
#include <string.h>

void something_big(void);

/*
 * References:
 * - https://stackoverflow.com/a/15350089
 * - https://gcc.gnu.org/onlinedocs/gcc/Function-Names.html
 */
int main(void)
{
	something_big();

	return 0;
}

/*
 * This identifier is not a preprocessor macro but a constant
 * variable from the compiler.
 */
void something_big(void)
{
	printf("__func__: %s\n", __func__);
	printf("__func__: %d\n", strlen(__func__));

	printf("__FUNCTION__: %s\n", __FUNCTION__);
	printf("__FUNCTION__: %d\n", strlen(__FUNCTION__));

	printf("__PRETTY_FUNCTION__: %s\n", __PRETTY_FUNCTION__);
	printf("__PRETTY_FUNCTION__: %d\n", strlen(__PRETTY_FUNCTION__));
}
