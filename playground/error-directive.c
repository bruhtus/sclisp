/*
 * References:
 * - https://gcc.gnu.org/onlinedocs/gcc-4.5.0/cpp/Diagnostics.html
 * - https://www.koderhq.com/tutorial/c/directive/
 */
int main(void)
{
#define ANU

	return 0;
}

#ifdef ANU
#error ANU
#endif

