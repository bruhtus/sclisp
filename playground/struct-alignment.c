#include <stdio.h>

/*
 * Looks like the member's position on the struct effect how
 * many memory we use because of struct alignment or padding.
 *
 * Still not sure how the padding arranged.
 *
 * Reference:
 * https://www.delftstack.com/howto/c/struct-alignment-in-c/
 */
struct first {
	char ini;
	int anu;
	short itu;
	int *nganu;
};

struct second {
	int anu;
	char ini;
	short itu;
	int *nganu;
};

struct third {
	int *nganu;
	char ini;
	short itu;
	int anu;
};

int main(void)
{
	printf("first struct: %zu\n", sizeof(struct first));
	printf("second struct: %zu\n", sizeof(struct second));
	printf("third struct: %zu\n", sizeof(struct third));

	printf("pointer type: %zu\n", sizeof(int *));
	printf("int type: %zu\n", sizeof(int));
	printf("short type: %zu\n", sizeof(short));
	printf("char type: %zu\n", sizeof(char));

	return 0;
}
