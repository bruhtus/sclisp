/*
 * Reference:
 * https://youtu.be/gVaXLlGqQ-c
 *
 * To include shared object in binary, we can use -static flag
 * like this:
 * gcc -static -o playground/printf-without-stdlib playground/printf-without-stdlib.c
 *
 * We can decrease the size when using -static flag
 * by using -nostdlib flag like this:
 * gcc -nostdlib -static -o playground/printf-without-stdlib playground/printf-without-stdlib.c
 *
 * If there's an error about `undefined reference to `__stack_chk_fail'`,
 * we can add -fstack-protector to exclude code for checking buffer overflows
 * like this:
 * gcc -nostdlib -static -fstack-protector -o playground/printf-without-stdlib playground/printf-without-stdlib.c
 */
int _start(void)
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

	asm volatile (
		"mov $60, %%rax\n\t" // SYS_EXIT.
		"mov $0, %%rdi\n\t" // exit code = 0.
		"syscall\n\t"
		:
		:
		: "%rax", "%rdi"
	);
}
