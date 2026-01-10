#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

char *msg_malloc(const char *, const char *, unsigned);

int main(void)
{
	char *msg = msg_malloc("Holy C!", __FILE__, __LINE__);
	printf("msg: %s", msg);
	free(msg);

	return 0;
}

/*
 * Reference:
 * https://www.dhairyashah.dev/posts/how-to-get-the-length-of-a-number-in-c/
 */
char *msg_malloc(
	const char *msg,
	const char *filename,
	unsigned line_number
)
{
	unsigned len_msg = strlen(msg) + 1;
	unsigned len_filename = strlen(filename) + 1;
	unsigned len_line_number = floor(log10(line_number)) + 1;

	unsigned limit;
	const char *fmt;

/*
 * Use `-D DEBUG` with gcc to get info filename and
 * line number.
 */
#ifdef DEBUG
	/*
	 * msg (playground/snprintf-with-malloc.c:69)\0\n
	 */
	limit = len_msg + 2 + len_filename + 1 + len_line_number + 2;
	fmt = "%s (%s:%u)\n";
#else
	/*
	 * msg\0\n
	 */
	limit = len_msg + 1;
	fmt = "%s\n";
#endif

	printf("message length: %u\n", limit);

	char *str = malloc(limit);

	int size_snprintf = snprintf(
		str,
		limit,
		fmt,
		msg,
		filename,
		line_number
	);

	printf("message length snprintf: %d\n", size_snprintf);

	return str;
}
