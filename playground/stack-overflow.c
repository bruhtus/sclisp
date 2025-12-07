/*
 * In linux (64-bit system), this will cause stack overflow
 * on the 10th iteration. We can check this using gdb with this
 * step (assumming we already inside gdb with command
 * `gdb playground/stack-overflow.c`):
 * - break 13 (add break points in line 13)
 * - `run`
 * - `continue 10`
 *
 * Reference:
 * https://medium.com/@lekushlev/understanding-and-demonstrating-stack-overflow-in-c-997975df7b59
 */
void function(void)
{
	char buf[1000000];
	function();
}

int main(void)
{
	function();
	return 0;
}
