#include <stdio.h>

/*
 * Reference:
 * https://cs.brown.edu/courses/csci0300/2021/notes/l05.html
 */
struct anu {
	int nganu;
	int ngitu;
};

int main(void)
{
	struct anu a = {
		.ngitu = 69,
		.nganu = 42,
	};

	struct anu *b = &a;

	printf("a = %d\n", a);
	printf("*b = %d\n", *b);
	printf("b->ngitu = %d\n", b->ngitu);

	return 0;
}
