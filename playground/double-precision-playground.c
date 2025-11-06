#include <stdio.h>
#include <float.h>

/*
 * References:
 * - https://stackoverflow.com/a/37830627
 * - https://www.cs.yale.edu/homes/aspnes/pinewiki/C(2f)FloatingPoint.html
 */
int main(void)
{
	double x = DBL_MAX;
	printf("%lf\n\n", x);

	x = x + 1E+292;
	printf("%lf\n", x);

	return 0;
}
