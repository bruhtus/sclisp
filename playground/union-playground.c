// Reference: https://www.tutorialspoint.com/cprogramming/c_unions.htm
#include <stdio.h>

union data {
	int i;
	float f;
	char *s;
};

int main(void)
{
	union data data;

	data.i = 69;
	data.f = 69.420;
	data.s = "C";

	printf("data.i: %d\n", data.i);
	printf("data.f: %f\n", data.f);
	printf("data.s: %s\n", data.s);

	printf("sizeof data: %d\n", sizeof(data));
	printf("sizeof data.i: %d\n", sizeof(data.i));
	printf("sizeof data.f: %d\n", sizeof(data.f));
	printf("sizeof data.s: %d\n", sizeof(data.s));
	return 0;
}
