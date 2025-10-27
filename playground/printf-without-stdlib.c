/*
 * Reference:
 * https://youtu.be/gVaXLlGqQ-c
 *
 * To include shared object in binary, we can use -static flag
 * like this:
 * gcc -static -o playground/printf-without-stdlib playground/printf-without-stdlib.c
 */
int main(void)
{
	const char *holy = "Holy C!\n";

	/*
	 * Reference:
	 * - %0 means: https://stackoverflow.com/a/10461830
	 * - embedded assembler: http://wiki.osdev.org/Inline_Assembly
	 */
	asm volatile (
		"mov $1, %%rax\n\t" // syscall number to write.
		"mov $1, %%rdi\n\t" // stdout (fd = 1).
		"mov %0, %%rsi\n\t" // use the first variable content.
		"mov $8, %%rdx\n\t" // characters length.
		"syscall\n\t"
		:
		: "m"(holy)
		: "%rax", "%rdi", "%rsi", "%rdx"
	);

	return 0;
}
