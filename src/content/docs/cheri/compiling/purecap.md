---
title: How do I compile for Purecap CHERI?
description: How do I compile for purecap CHERI?
---

This page deals with _purecap_ compilation for native capability code.
There is a separate page for [\*hybrid\* compilation](/cheri/compiling/hybrid).

If you have access to Morello hardware, you can run the LLVM compiler natively on CheriBSD. You need to install llvm with pkg, then you can run it as you would on any other platform.

```bash
pkg64 install llvm-morello
```

Then you can invoke the clang compiler to compile c programs to purecap binaries like this:

```bash
clang-morello -march=morello -mabi=purecap ./file.c
```

You need to specify both the march and mabi flags explicitly for purecap mode.

<!-- TODO should this really live in this file? -->

However, if you're using an old version of _clang-morello_ and trying to pass a capability pointer to a variadic function, such as _printf_, you might still encounter the _SIGPROT_ fault.

Let's say we have the following code, where `my_func` is a variadic function that you'd like to invoke with some capability pointers:

```c
// variadic.c
#include <stdarg.h>
#include <stdio.h>
#include <string.h>

size_t my_func(int num, ...) {
   va_list args;
   va_start(args, num);
   size_t len = 0;
   for (int i = 0; i < num; i++) {
      len += strlen(va_arg(args, char *));
   }
   va_end(args);
   return len;
}

int main() {
   size_t bytes = my_func(3, "str1", "this is str2", "and str3");
   printf("Total bytes: %zu\n", bytes);
}
```

And if you compile the code with the compilation command above and run
the output binary, you\'ll see the following output:

```bash
$ clang-morello -march=morello -mabi=purecap ./variadic.c
$ ./a.out
In-address space security exception (core dumped)
```

To fix this, we have to pass an extra argument, `-Xclang-morello-vararg=new`, to clang to indicate that we intend to use variadic functions with capability pointers:

```bash
$ clang-morello -march=morello -mabi=purecap -Xclang -morello-vararg=new ./variadic.c
$ ./a.out
Total bytes: 24
```

## Which compiler?

The Cambridge CHERI team support the LLVM compiler. This can target various CHERI architectures including RISC-V and Morello. This is the default compiler for the CheriBSD OS.

The Arm Morello development team have a GCC compiler port.
