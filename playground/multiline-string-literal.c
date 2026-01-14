#include <stdio.h>

/*
 * Reference:
 * https://stackoverflow.com/a/1135862
 */
int main(void)
{
	const char *anu =
		"anu1 "
		"anu2 "
		"anu3"
	;

	printf("anu: %s\n", anu);

	const char *itu = "\
		itu1 \
		itu2 \
		itu3 \
		"
	;

	printf("itu: %s\n", itu);

	const char *ini = "\
ini1 \
ini2 \
ini3 \
";

	printf("ini: %s\n", ini);

	return 0;
}
