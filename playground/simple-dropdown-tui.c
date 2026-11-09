/*
 * Reference:
 * https://dev.to/sammaji/create-dropdown-menu-tui-in-c-485b
 */
#include <stdlib.h>
#include <stdio.h>
#include <signal.h>
#include <termios.h>
#include <unistd.h>

/*
 * References:
 * - https://en.wikipedia.org/wiki/ANSI_escape_code#Control_Sequence_Introducer_commands
 * - https://ansi.tools/lookup
 * - https://gist.github.com/fnky/458719343aabd01cfb17a3a4f7296797
 *
 * Opening square bracket ([) known as Control Sequence Introducer (CSI).
 */
#define CLEAR_SCREEN "\033[H\033[2J" // Move cursor to top left and scroll down until previous content not visible (?).
#define HIGHLIGHT "\033[7m" // Reverse the color.
#define RESET_COLOR "\033[m" // Looks like this is equivalent to \033[0m (?).

#define KEY_UP 65 // ASCII code A (for '\033[A').
#define KEY_DOWN 66 // ASCII code B (for '\033[B').
#define KEY_j 106 // ASCII code j (for vim key down).
#define KEY_k 107 // ASCII code k (for vim key up).
#define KEY_ENTER 10
#define KEY_ESC 27
#define KEY_q 113

#define ARR_LEN(arr) (sizeof(arr) / sizeof(*arr))

struct dropdown_type {
	const char *const *opts;
	int num_opts;
	int selected;
	const char *title;
};

static void init_terminal(void);
static void reset_terminal(void);

static struct dropdown_type *create_dropdown(const char *, const char *const *, int);
static void display_dropdown(struct dropdown_type *);

static char read_key(void);

static void handle_move_key(struct dropdown_type *, char);
static int run_dropdown(struct dropdown_type *);

int main(void)
{
	struct sigaction new_sigact;
	int retval;

	const char *title = "Select an option (press ESC or q to exit):";
	const char *const opts[] = {
		"Option 1",
		"Option 2",
		"Option 3",
		"Option 4",
	};
	int num_opts = ARR_LEN(opts);

	new_sigact.sa_handler = SIG_IGN;
	new_sigact.sa_flags = 0;

	/*
	 * Ignore signal interrupt (ctrl-c).
	 * Looks like we can still use ctrl-c in gdb to interrupt the program (?).
	 */
	retval = sigaction(SIGINT, &new_sigact, NULL);
	if (retval == -1) {
		perror("sigaction() failed");
		return 69;
	}

	struct dropdown_type *dropdown = create_dropdown(title, opts, num_opts);
	if (!dropdown)
		return 69;

	int selected = run_dropdown(dropdown);
	if (selected == -1) {
		printf("%sSelection canceled\n", CLEAR_SCREEN);
		goto cleanup;
	}

	printf("%sYou selected: %s\n", CLEAR_SCREEN, opts[selected]);

cleanup:
	free(dropdown);

	return 0;
}

static void init_terminal(void)
{
	struct termios term;
	int retval;

	retval = tcgetattr(STDIN_FILENO, &term);
	if (retval) {
		perror("tcgetattr() failed");
		exit(1);
	}

	/*
	 * 0000 0000
	 * 0000 0001
	 * 0000 0010 -> ICANON
	 * 0000 0011
	 * 0000 0100
	 * 0000 0101
	 * 0000 0110
	 * 0000 0111
	 * 0000 1000 -> ECHO
	 * 0000 1001
	 * 0000 1010 -> ICANON | ECHO -> 10
	 * 1111 0101 -> ~(ICANON | ECHO) -> ~10 -> -11
	 *
	 * 00000000000000001000101000111011 -> c_lflag
	 * 11111111111111111111111111110101
	 * -------------------------------- &
	 * 00000000000000001000101000110001
	 */
	term.c_lflag &= ~(ICANON | ECHO);

	retval = tcsetattr(STDIN_FILENO, TCSANOW, &term);
	if (retval) {
		perror("tcsetattr() failed");
		exit(1);
	}
}

static void reset_terminal(void)
{
	struct termios term;
	int retval;

	retval = tcgetattr(STDIN_FILENO, &term);
	if (retval) {
		perror("tcgetattr() failed");
		exit(1);
	}

	/*
	 * 0000 0000
	 * 0000 0001
	 * 0000 0010 -> ICANON
	 * 0000 0011
	 * 0000 0100
	 * 0000 0101
	 * 0000 0110
	 * 0000 0111
	 * 0000 1000 -> ECHO
	 * 0000 1001
	 * 0000 1010 -> ICANON | ECHO -> 10
	 *
	 * 00000000000000001000101000110001 -> c_lflag
	 * 00000000000000000000000000001010
	 * -------------------------------- |
	 * 00000000000000001000101000111011
	 */
	term.c_lflag |= (ICANON | ECHO);

	retval = tcsetattr(STDIN_FILENO, TCSANOW, &term);
	if (retval) {
		perror("tcsetattr() failed");
		exit(1);
	}
}

static struct dropdown_type *create_dropdown(
	const char *title,
	const char *const *opts,
	int num_opts
)
{
	struct dropdown_type *dropdown = malloc(sizeof(*dropdown) * 1);
	if (!dropdown)
		return NULL;

	dropdown->opts = opts;
	dropdown->num_opts = num_opts;
	dropdown->selected = 0;
	dropdown->title = title;

	return dropdown;
}

/*
 * The problem with this implementation is that the previous dropdown display
 * is still available if we scroll up. Still not sure how to directly update
 * selection without leaving the previous dropdown display state at the top
 * of current state.
 */
static void display_dropdown(struct dropdown_type *dropdown)
{
	int i;

	printf("%s%s\n\n", CLEAR_SCREEN, dropdown->title);

	for (i = 0; i < dropdown->num_opts; ++i) {
		if (i == dropdown->selected)
			printf("%s> %s%s\n", HIGHLIGHT, dropdown->opts[i], RESET_COLOR);
		else
			printf("  %s\n", dropdown->opts[i]);
	}
}

static char read_key(void)
{
	char key[3];
	ssize_t read_retval;

	/*
	 * When arrow up key is pressed, the key will be:
	 * - "\033" (ESC key)
	 * - "["
	 * - "A"
	 */
	read_retval = read(STDIN_FILENO, &key, ARR_LEN(key));
	if (read_retval == -1) {
		perror("read() failed");
		exit(1);
	}

	if (read_retval == 3)
		return key[2]; // Return the key after "\033[".

	return key[0];
}

static void handle_move_key(struct dropdown_type *dropdown, char key)
{
	switch (key) {
		case KEY_UP:
		case KEY_k:
			if (dropdown->selected > 0)
				--dropdown->selected;
			break;

		case KEY_DOWN:
		case KEY_j:
			if (dropdown->selected < (dropdown->num_opts - 1))
				++dropdown->selected;
			break;
	}

	display_dropdown(dropdown);
}

/*
 * The problem with the original reference is that we constantly read the key,
 * which can cause pressing the key once might not be enough, especially with
 * the ESC key, arrow up, and arrow down key. So trying to minimize reading
 * the key to make it more responsive (?).
 */
static int run_dropdown(struct dropdown_type *dropdown)
{
	char key;

	init_terminal();
	display_dropdown(dropdown);

	for (;;) {
		key = read_key();

		switch (key) {
			case KEY_DOWN:
			case KEY_j:
				handle_move_key(dropdown, key);
				break;

			case KEY_UP:
			case KEY_k:
				handle_move_key(dropdown, key);
				break;

			/*
			 * Confirm selection.
			 */
			case KEY_ENTER:
				goto cleanup;

			case KEY_ESC:
			case KEY_q:
				dropdown->selected = -1;
				goto cleanup;
		}
	}

cleanup:
	reset_terminal();

	return dropdown->selected;
}
