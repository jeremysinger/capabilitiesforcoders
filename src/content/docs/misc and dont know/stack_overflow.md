---
title: Stack Overflow Prevention
description: Stack Overflow Prevention
---

<!-- TODO Is this a warning/error? -->

## Stack Overflow

Local variables are stored on the stack. The stack is a contiguous block
of memory that grows and shrinks as functions are called and return.
When dealing with user input, it is possible to overflow the stack by
providing too much input. This can be used to overwrite other data on
the stack. This is a common method of exploiting buffer overflows.

For example, consider the following program:

```C
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#ifdef __morello__
#define PTR_FORMAT "%#p"
#else
#define PTR_FORMAT "%p"
#endif

int my_strcpy(char * buf, const char * src){
    int i = 0;
    if (!src) return i;

    char * cur = buf;
    while(src[i] != '\0'){
        printf("write to cur: " PTR_FORMAT "\n", cur);
        *cur = src[i];
        cur++;
        i++;
    }
    printf("write to cur: " PTR_FORMAT "\n", cur);
    *cur = '\0';
    return i;
}

int main(int argc, char **argv){
    int i = 1;
    char buffer[4];

    printf("buffer: " PTR_FORMAT "\n", buffer);
    printf ("i = %d (" PTR_FORMAT ")\n", i, &i);
    printf("Change i's value from 1 -> 500.\n");
    my_strcpy(buffer, argv[1]);

    if (i == 65) {
        printf("GOOD\n");
        system("/bin/sh");
    }

    printf("buffer: [%s] (%zu)\n", buffer, strlen(buffer));
    printf ("i = %d (" PTR_FORMAT ")\n", i, &i);
    return 0;
}
```

When compiling it with the following command on a non-CHERI system,

```bash
$ clang-morello -O0 -g stack_overflow.c -o stack_overflow_unprotected
```

and executing it with `AAA` as shown below, it works as expected:

```shell
$ ./stack_overflow_unprotected AAA
buffer: 0x809fa948
i = 1 (0x809fa94c)
Change i\'s value from 1 -> 500.
write to cur: 0x809fa948
write to cur: 0x809fa949
write to cur: 0x809fa94a
write to cur: 0x809fa94b
buffer: [AAA] (3)
i = 1 (0x809fa94c)
```

However, when executing it with `AAAAA`, it overwrites the value of `i`:

```shell
$ ./stack_overflow_unprotected AAAAA
buffer: 0x81036528
i = 1 (0x8103652c)
Change i\'s value from 1 -> 500.
write to cur: 0x81036528
write to cur: 0x81036529
write to cur: 0x8103652a
write to cur: 0x8103652b
write to cur: 0x8103652c
write to cur: 0x8103652d
GOOD
$ # We are now in a shell spawned by the program
$ ^D
buffer: [AAAAA] (5)
i = 65 (0x8103652c)
```

The program has a buffer overflow vulnerability. The program copies the
first argument using a custom `my_strcpy` function to display the
address it is modifying. The function copies the string character by
character until it reaches the end.

Normally, it\'s the programmer\'s responsibility to ensure that the
destination buffer is large enough to hold the source string, or using a
safe function like `strncpy`. However, in this case, the programmer has
made a mistake and the buffer is too small to hold the string. This
means that the program will write past the end of the buffer,
overwriting the value of `i`.

## Vulnerability Mitigration with Morello

The Morello architecture tags the allocated memory and restrict any
illegal access outside the designated bound. So the program will crash
when it tries to write to an invalid address. Let\'s compile the program
with the Morello compiler:

```shell
# compile natively on a Morello system
$ clang-morello -O0 -g -D__morello__ \
    -march=morello -mabi=purecap \
    -Xclang -morello-vararg=new \
    stack_overflow.c -o stack_overflow

# cross-compile with LLVM toolchain and musl libc
# LLVM toolchain: https://git.morello-project.org/morello/llvm-project-releases
# musl libc: https://git.morello-project.org/morello/musl-libc
$ clang -O0 -g -D__morello__ \
    -march=morello --target=aarch64-linux-musl_purecap \
    --sysroot=/root/musl-sysroot-purecap \
    stack_overflow.c -o stack_overflow -static
```

and execute it with `AAAAA`:

```shell
# run on a Morello system natively
$ ./stack_overflow AAAAA

# OR
# run on a Morello system with Morello Instruction Emulator, morelloie
# morelloie: https://developer.arm.com/downloads/-/morello-instruction-emulator
$ morelloie -- ./stack_overflow AAAAA

buffer: 0xfffffff7ff58 [rwRW,0xfffffff7ff58-0xfffffff7ff5c]
i = 1 (0xfffffff7ff5c [rwRW,0xfffffff7ff5c-0xfffffff7ff60])
Change i\'s value from 1 -> 500.
write to cur: 0xfffffff7ff58 [rwRW,0xfffffff7ff58-0xfffffff7ff5c]
write to cur: 0xfffffff7ff59 [rwRW,0xfffffff7ff58-0xfffffff7ff5c]
write to cur: 0xfffffff7ff5a [rwRW,0xfffffff7ff58-0xfffffff7ff5c]
write to cur: 0xfffffff7ff5b [rwRW,0xfffffff7ff58-0xfffffff7ff5c]
write to cur: 0xfffffff7ff5c [rwRW,0xfffffff7ff58-0xfffffff7ff5c]
In-address space security exception (core dumped)
```

The program crashes with an in-address space security exception. This is
because the program is trying to write to an address that is not in the
address space of the buffer pointer. Although the value of `cur` was
changed to `0xfffffff7ff5c`, the original pointer is restricted in the
address space `0xfffffff7ff58-0xfffffff7ff5c` (not including the last
byte). This means that, any pointer derived from the original `buffer`
pointer will also be limited to that address space, and the program will
crash when it tries to write outside that address space.
