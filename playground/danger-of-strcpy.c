#include <stdio.h>
#include <string.h>

int main(void)
{
	char ini[2];

	/*
	 * The expected scenario is
	 * that the position of
	 * these data in memory is
	 * side by side, like this:
	 * ..., ini[], anu[], itu[], ...
	 */
	char anu[] = {'a', 'b'};
	char itu[] = {'c', '\0'};

	/*
	 * strcpy() will copy the string
	 * until it found null terminator
	 * character (\0), which means
	 * that if we forgot to null
	 * terminated our string, strcpy()
	 * will copy _how many characters_
	 * until we got the null terminator
	 * character.
	 */
	strcpy(ini, anu);

	/*
	 * Notice that we are replacing
	 * the data in anu[] memory
	 * segments?
	 */
	printf("ini[2]: %c\n", ini[2]);
	printf("anu[0]: %c\n", anu[0]);

	printf("ini: %d\n", ini[3] == '\0');
	printf("anu address: %p\n", anu);
	printf("anu+1 address: %p\n", anu + 1);
	printf("ini address: %p\n", ini);
	printf("ini+3 address: %p\n", ini + 3);
	printf("itu address: %p\n", itu + 1);

	return 0;
}
