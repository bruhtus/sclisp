#include <stdio.h>
#include <string.h>

/*
 * Reference:
 * https://www.gnu.org/software/c-intro-and-ref/manual/html_node/Predefined-Macros.html
 */
int main(void)
{
	printf("__FILE__: %s\n", __FILE__);
	printf("__FILE__ strlen: %d\n", strlen(__FILE__));
	printf("__FILE__ sizeof: %d\n", sizeof(__FILE__));

	/*
	 * Seems like __FILE__ expands to string literal and
	 * possibly have the static storage duration too (?).
	 */
	printf("__FILE__ address: %p\n", __FILE__);
	printf(
		"filename string literal address: %p\n",
		"playground/length-file-line-macro.c"
	);

	/*
	 * Reference:
	 * https://stackoverflow.com/questions/8257714/how-can-i-convert-an-int-to-a-string-in-c#comment53469384_8257728
	 */
	int line_len = snprintf(NULL, 0, "%d", __LINE__);

	printf("__LINE__: %d\n", __LINE__);
	printf("__LINE__ length: %d\n", line_len);

	return 0;
}
