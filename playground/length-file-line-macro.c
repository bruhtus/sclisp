#include <stdio.h>
#include <string.h>

int main(void)
{
	printf("__FILE__: %s\n", __FILE__);
	printf("__FILE__: %d\n", strlen(__FILE__));

	/*
	 * Reference:
	 * https://stackoverflow.com/questions/8257714/how-can-i-convert-an-int-to-a-string-in-c#comment53469384_8257728
	 */
	int line_len = snprintf(NULL, 0, "%d", __LINE__);

	printf("__LINE__: %d\n", __LINE__);
	printf("__LINE__: %d\n", line_len);

	return 0;
}
