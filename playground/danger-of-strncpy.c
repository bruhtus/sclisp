#include <stdio.h>
#include <string.h>

#define ARRAY_LEN(array) (sizeof(array) / sizeof(*array))

int main(void)
{
	char ini[2];

	char anu[] = {'a', 'b'};

	/*
	 * There's no guarantee that strncpy()
	 * will have null terminated string,
	 * especially if the original string
	 * does not end in null terminated
	 * or the destination buffer is less
	 * than the original string.
	 */
	strncpy(ini, anu, ARRAY_LEN(ini));

	printf(
		"ini[1] is null terminator? %s\n",
		ini[1] == '\0' ? "Yes" : "No"
	);

	return 0;
}
