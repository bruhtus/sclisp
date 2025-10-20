/*
 * We can also using the another macro definition as a value,
 * and the value of another macro definition will be replaced
 * during preprocessing process.
 *
 * And it seems like the order of macro definition does not
 * matter (?), still not sure if this is correct or not.
 *
 * We can also define macro in multiple lines using
 * backslash (\)
 *
 * Macro only replacing the value and can not do arithmetic
 * operation. So the macro below is only replacing LIFE with
 * value of (NICE - 27) and NICE will be replaced by value 69.
 * So the end result would be (69 - 27) and the arithmetic
 * operation will be done when we are evaluate the value like
 * using printf() to print out the value.
 */
#define LIFE \
	(NICE - 27)

/*
 * Every exact word (case-sensitive and no extra characters)
 * in macro definition will be replaced by the value.
 *
 * In this case, every word NICE will be replaced by value 69.
 */
#define NICE 69

/*
 * On linux, we can check the preprocessed code of this file
 * with command:
 * cpp macro-playground.c
 */
int main(void)
{
	int something = NICE;
	int life = LIFE;
}
