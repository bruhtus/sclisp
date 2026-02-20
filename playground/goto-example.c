#include <stdio.h>

int arithmatic(char, int, int);

/*
 * Labels have __function scope__, so if it's in another
 * block scope like in if-else block scope or looping
 * block scope, we can't use the variable in those scope.
 *
 * References:
 * - https://beej.us/guide/bgc/html/split/goto.html
 * - https://www.kernel.org/doc/html/v4.10/process/coding-style.html#centralized-exiting-of-functions
 */
int main(void)
{
	int i;
	int life = 69;

	printf("one\n");
	printf("two\n");

	/*
	 * We can't access `num` variable in the label
	 * skip_3 because it's in different scope.
	 */
	if (life == 42) {
		int num = arithmatic('+', 1, 1);
		printf("num: %d\n", num);
		goto skip_3;
	}

	/*
	 * We also can't access `num` variable from
	 * this for-loop in label skip_3 because it's in different
	 * scope.
	 */
	for (i = 0; i < 3; ++i) {
		int num = arithmatic('-', 1, 1);
		printf("num: %d\n", num);
		goto skip_3;
	}

	printf("three");

	return 0;

	/*
	 * Looks like we can't really use macro __LINE__
	 * to get the line number from where the label
	 * used.
	 */
skip_3:
	printf("line: %d\n", __LINE__);
	return 42;
}

int arithmatic(char op, int a, int b)
{
	if (op == '+')
		goto add;

	goto minus;

	/*
	 * Putting this at end of function so that
	 * we didn't accidentally execute the statement
	 * inside the label.
	 *
	 * Because if didn't put the label at the end
	 * of the function, the execution will ignore
	 * the label and just execute the statements
	 * inside the label. It's like when we didn't
	 * put `break` statement in the switch-case
	 * statement and we are falling through to the
	 * next label.
	 */
add:
	return a + b;
minus:
	return a - b;
}
