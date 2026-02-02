#include <stdio.h>

struct something {
	int num;
	char c;
};

int main(void)
{
	struct something big[1] = {
		{69, 'E'}
	};

	printf("%d\n", big[0].num);

	/*
	 * Usually we need to assign each struct's element
	 * of an array one by one to change the value
	 * of those struct's element, like this:
	 * ```
	 * big[0].num = 42;
	 * big[0].c = '*';
	 * ```
	 *
	 * But since C99, we can use compound literal that
	 * will create _unnamed object_ that we can use
	 * to replace all of the struct's element value of
	 * an array.
	 *
	 * References:
	 * - https://stackoverflow.com/a/1223806
	 * - https://en.cppreference.com/w/c/language/compound_literal.html
	 */
	big[0] = (struct something){42, '*'};
	printf("%d\n", big[0].num);

	return 0;
}
