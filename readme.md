# Sclisp

Sclisp is a toy project for learning C programming language. It's mostly based
on the book [Build Your Own Lisp](https://www.buildyourownlisp.com/) and using
[mpc parser](https://github.com/orangeduck/mpc) to parse the characters.

Keep in mind that this project purpose is to learn C programming language
rather than creating a full fledge lisp interpreter, so the features
implemented in this project is only a small subset of what other lisp
interpreter have.

There's a few tweak that didn't exist in the book, like malloc() failure
handler and using the archive library, see [Highlights](#highlights) for more
tweaks info. There's also the `playground` directory which contains my
experiment about call stack overflow, array in function parameter, `gcc`
builtin arithmetic overflow function, and so on.

The source code mostly using the linux kernel coding style as guidance, like
no curly bracket if there's only one statement in `if-else` statement or
using tab instead of space for indentation.

## Running the REPL

You can compile the source code by running the `run` script like:
```sh
./run main
```

Or if you need some debugging information, you can use this:
```sh
./run main-debug
```

If you make any changes to the `mpc` parser, you can recompile the archive
library with this:
```sh
./run mpc
```

After you compile the source code, you can start the REPL like this:
```sh
./sclisp
```

## Caveats

There's a few caveats with sclisp and some them are:
1. Sclisp only tested in arch linux. So if you use another linux distro or
   even another operating system, you might need to tweak the source code or
   the `run` script, like the linking to readline library.

2. Sclisp using double type for the result. This is to make parsing easier by
   only using 1 type for the number, which can cause a problem.

3. Sclisp only tested using `gcc` compiler, so the code might not compile when
   using other compiler.

4. Sclisp depends on operating system to clean up the allocated memory when
   there's allocation failure or integer overflow, because we exit the program
   directly without doing the clean up of previous memory allocation. This is
   bad but i'm still not sure what is the right mechanism to do that with the
   interpreter.

There might be other caveats that i didn't realize yet, so be careful.

## Highlights

This is the highlight of what i did differently from the book:
1. Separate the source code to `sclisp.c`, `utils.c`, `utils.h`. Trying to
   emulate the library structure with `utils.c` and `utils.h`.

2. Rather than compiling the `mpc` parser over and over again, we compile
   it once into an object file and then put that as archive library. This can
   safe some time during compilation process, especially if we did not change
   the parser frequently.

3. Handle case when allocation with `malloc()` or `realloc()` failed, which
   is returning `NULL` value, by exit the program with code 69.

4. Handle integer overflow case with `gcc` builtin arithmetic overflow
   function such as `__builtin_uadd_overflow()` and `__builtin_addc()` and
   exit the program with code 42.

5. Trying to implement `reallocarray()` from the BSD system, which basically
   `realloc()` with multiplication overflow checker (still not sure if this is
   the right implementation).

6. Changing the `mpca_lang()` and `mpc_cleanup()` from the `mpc` parser to
   take array and array size instead of variadic function. The problem with
   using variadic function with `mpca_lang()` is that when we forgot to put
   `NULL` at the end of the arguments and there's unknown parser name, we will
   get segmentation fault error. That's because there's no limit when we try
   to find the parser with `mpca_grammar_find_parser()` which will attempt to
   access some inaccessible memory. And by using array and array size in
   `mpc_cleanup()`, we don't need to update the arguments every time we add
   new parser as long as we use the same array as `mpca_lang()`.

7. Trying to implement copy string functionality to replace `strcpy()`, called
   `stringcpy()`.

8. Handle the truncated value scenario of `snprintf()`.

This is the things that i remember doing differently. There might be other
things that i forgot to mention, so if you want to find out more, you can
check the source code.

## Examples

```sh
sclisp> head {69 42 69420}
sclisp> (head {69 42 69420})
sclisp> (tail {69 42 69420})
sclisp> (len {69 42 69420})
sclisp> eval (tail {tail head (list 69 42 69420)})
sclisp> join {69} {69420 69} (eval {head (list 42 69420)})
sclisp> let {args} {x y}
sclisp> let args 69 42
sclisp> (+ x y)
sclisp> % 69 0
sclisp> / 69 0
sclisp> ^ 2 3
sclisp> * 2 3
sclisp> - 2 3
```

## Side Notes

This project was mostly built on live stream at youtube. For the playlist, you
can check
[here](https://www.youtube.com/playlist?list=PL0Uvin-so_c9eXp_ugGp6V5U-QpdZq0C7).

This project stopped at Chapter 11 from the book "Build Your Own Lisp" mainly
because i have some doubt about the next chapter. For example, in Chapter 12
we try to implement a lambda function but the book's implementation can not
handle a lambda function without a parameter. At the time of building this
project, i have no experience in lisp so i am not sure if that is expected or
not.

## References

- [Build your own lisp book](https://www.buildyourownlisp.com/).
- [MPC parser](https://github.com/orangeduck/mpc).
- [Linux kernel coding
style](https://www.kernel.org/doc/html/v4.10/process/coding-style.html).
- [C reference](https://en.cppreference.com/w/c.html).
- [C99
rationale pdf](https://www.open-std.org/jtc1/sc22/wg14/www/C99RationaleV5.10.pdf).
- [GNU history library](https://tiswww.case.edu/php/chet/readline/history.html).
- [Where C string literal stored](https://stackoverflow.com/a/30662565).
- [Examine contents of data section in ELF
file](https://stackoverflow.com/q/1685483).
- [Memory segments in C/C++ (youtube
video)](https://youtu.be/2htbIR2QpaM?si=CbcBjQ1NvKiZRRoR).
- [Understand the .bss segment in
C](https://chessman7.substack.com/p/understanding-the-bss-segment-in).
- [History about virtual memory (youtube
video)](https://youtu.be/vc79sJ9VOqk?si=EXR-UUisqKv93PbK).
- [Linux kernel programming language
documentation](https://www.kernel.org/doc/html/latest/process/programming-language.html).
- [x86 assembly
guide](https://www.cs.virginia.edu/~evans/cs216/guides/x86.html).
- [Inline assembly in C](https://wiki.osdev.org/Inline_Assembly).
- [Basic integer overflow](https://phrack.org/issues/60/10).
- [GCC string literal
(pdf)](http://www.cs.columbia.edu/~ecj2122/research/gcc_string_literals/gcc_string_literals.pdf).
- [C programming static keyword](https://software.codidact.com/posts/285050).
- [Build C static/archive
library](https://dev.to/iamkhalil42/all-you-need-to-know-about-c-static-libraries-1o0b).
- [Function call stack
demo](https://web.archive.org/web/20221025071830/https://cs.gmu.edu/~kauffman/cs222/stack-demo.html).
- [GNU virtual memory
documentation](https://ftp.gnu.org/old-gnu/Manuals/glibc-2.2.3/html_chapter/libc_3.html).
- [Computer science from the bottom up](https://bottomupcs.com/index.html).
- [right-left rule to read C
declaration](https://cseweb.ucsd.edu/~ricko/rt_lt.rule.html).
- [Explanation about pointer](https://cplusplus.com/doc/tutorial/pointers/).
- [Explanation about function pointer (youtube
video)](https://youtu.be/f_uWOWViYc0?t=413).
- [GDB Online
Documentation](https://sourceware.org/gdb/current/onlinedocs/gdb.html/index.html).
- [Big endian vs little endian
format](https://open4tech.com/little-endian-vs-big-endian-in-embedded-systems/).
- [GNU C library (glibc) source
code](https://sourceware.org/git/?p=glibc.git;a=tree).
- [FreeBSD C library source
code](https://github.com/freebsd/freebsd-src/tree/main/lib/libc).
