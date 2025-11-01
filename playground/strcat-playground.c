/*
 * Reference:
 * https://www.joelonsoftware.com/2001/12/11/back-to-basics/
 */
#include <stdio.h>

char *my_strcat(char *dest, char *src);

int main(void)
{
	char big_string[69];
	big_string[0] = '\0';

	char *p = big_string;

	p = my_strcat(p, "anu, ");
	p = my_strcat(p, "itu, ");
	p = my_strcat(p, "inv");

	printf("%s\n", big_string);

	return 0;
}

char *my_strcat(char *dest, char *src)
{
	/*
	 * This means that we are moving the dest
	 * character to the null terminator (\0).
	 */
	while (*dest)
		dest++;

	/*
	 * This means that we are replacing the
	 * null terminator (\0) on dest with the
	 * first character of src and advancing
	 * into the next memory space in dest and
	 * next src character.
	 *
	 * This looping end when src character is null
	 * terminator (\0) and assigned that character
	 * to dest.
	 *
	 * Reference:
	 * https://stackoverflow.com/a/151870
	 */
	while ((*dest = *src) != '\0') {
		printf("*src: %c\n", *(src + 0));

		dest++;
		src++;
	}

	printf("null dest: %d\n", *dest == '\0');

	/*
	 * We are returning the last character pointer
	 * so that we don't need to re-scanning the new
	 * string from the beginning again.
	 *
	 * Preventing Shlemiel the painter’s algorithm.
	 */
	return --dest;
}
