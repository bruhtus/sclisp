int *something()
{
	/* static int a = 69; */
	int a = 69; // The virtual memory of this variable get freed after the function return.
	return &a;
}

int main()
{
	/* char *str = "ANU"; */
	/* str[0] = 'B'; */

	/* int *a, b; */
	/* /1* a = &b; *1/ */
	/* *a = 69; */

	int *a = something();
	*a = 42;

	return 0;
}
