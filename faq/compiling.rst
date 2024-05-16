=============================
 How do I compile for CHERI?
=============================

This page deals with *purecap* compilation for native
capability code. There is a separate page
for :ref:`*hybrid* compilation <faq/compiling_hybrid>`.

Cross-compilation
=================

The LLVM compiler runs in cross-compilation mode for CHERI, so you can compile to CHERI targets from a non-CHERI platform like x86_64/linux.
If you use cheribuild, then by default your CHERI LLVM toolchain will be
located in the `cheri` directory in your home directory. There is a
configuration file with the relevant parameters set up to enable cross-compilation. One convenient way to
invoke the cross-compiler is by defining appropriate environment variables:

.. code-block:: bash

   export CC=~/cheri/output/morello-sdk/bin/clang
   export CFLAGS="--config cheribsd-morello-purecap.cfg"

You can then invoke the cross-compiler on your non-CHERI platform:

.. code-block:: bash

   $CC $CFLAGS test.c

which will generate an `a.out` executable file that you can copy to a
CHERI system to execute.

Cross-compilation is most useful when you are running a CHERI emulator and
the native CHERI compiler runs too slowly on the emulator.




Native compilation
==================

If you have access to Morello hardware, you can run the LLVM compiler natively
on CheriBSD. You need to install llvm with pkg, then you can run it as you would on any other platform.

.. code-block:: bash

   pkg64 install llvm-morello

Then you can invoke the clang compiler to compile c programs
to purecap binaries like this:

.. code-block:: bash

   clang-morello -march=morello -mabi=purecap ./file.c

You need to specify both the march and mabi flags explicitly for
purecap mode.

However, if you're using an old version of `clang-morello` and 
trying to pass a capability pointer to a varidiac function, such
as `printf`, you might still encounter the `SIGPROT` fault.

Let's say we have the following code, where `my_func` is a varidiac
function that you'd like to invoke with some capability pointers:

.. code-block:: c

   // varidiac.c
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


And if you compile the code with the compilation command above and 
run the output binary, you'll see the following output:

.. code-block:: bash

   $ clang-morello -march=morello -mabi=purecap ./varidiac.c
   $ ./a.out
   In-address space security exception (core dumped)


To fix this, we have to pass an extra argument, `-Xclang -morello-vararg=new`,
to clang to indicate that we intend to use varidiac functions with
capability pointers:

.. code-block:: bash

   $ clang-morello -march=morello -mabi=purecap -Xclang -morello-vararg=new ./varidiac.c
   $ ./a.out
   Total bytes: 24


Which compiler?
===============

The Cambridge CHERI team support the LLVM compiler. This can target various CHERI architectures including RISC-V and Morello. This is the default compiler for the CheriBSD OS.

The Arm Morello development team have a GCC compiler port.

