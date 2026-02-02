#include <stdio.h>

int main(void)
{
	char buf[3] = {'a', 'b', '\0'};

	char *const str = buf;
	printf("%s\n", str);

	/*
	 * We can initialize a constant variable but
	 * we can't reassign those constant variable
	 * with another memory address.
	 */
	/* str = "xy"; */
	/* printf("%s\n", str); */

	/*
	 * So the constant effecting the pointer instead of
	 * the type that the pointer points to. In the above
	 * example, we can not change the memory address saved
	 * in the pointer, but we can change the character
	 * that inside those memory address.
	 *
	 * This is different from `const char *str`, because
	 * the constant is on the type that the pointer
	 * points to instead of the memory address that was
	 * saved in the pointer. That means, in case of
	 * `const char *str`, we can replace the memory
	 * address, like `str = &new`, but we can't change the
	 * character inside those memory address, like
	 * `str[0] = 'y'`.
	 */
	str[0] = 'x';
	printf("%s\n", str);

	int num = 69;
	int num2 = 42;

	int *const num_ptr = &num;

	/*
	 * This is also the same for other types too.
	 */
	/* num_ptr = &num2; */

	return 0;
}
