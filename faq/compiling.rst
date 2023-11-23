=============================
 How do I compile for CHERI?
=============================



Cross-compilation
=================

The LLVM compiler runs in cross-compilation mode for CHERI, so you can compile to CHERI targets from a non-CHERI platform like x86_64/linux.

- If you use cheribuild, then by default your CHERI LLVM toolchain will be
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

- You can also use some non-official MorelloIE Docker images, such as ``cocoaxu/morelloie-llvm``
  and ``cocoaxu/morelloie-gcc``. They contain the LLVM-toochain and GCC-toolchain for CHERI, 
  respectively. You can use them to compile your CHERI programs on your non-CHERI platform.
  
  Meanwhile, you can also use the ``--volume`` option to mount your source code directory
  into the container. For example, if your source code is in ``/home/user/src``, you can do
  the following:

  .. code-block:: shell

    # using LLVM toolchain with musl libc
    $ docker run -it --rm --volume /home/user/src:/src cocoaxu/morelloie-llvm

    # using GCC toolchain with glibc
    $ docker run -it --rm --volume /home/user/src:/src cocoaxu/morelloie-gcc

  Then you can compile your CHERI programs inside the container. Inside these Docker images,
  they both have ``morelloie`` pre-installed while the CHERI compiler is ``clang`` and ``gcc``,
  respectively. 
  
  And for the LLVM toolchain specificly, a sysroot directory for Purecap can be found at 
  ``/root/musl-sysroot-purecap``, which can be used later when compiling Purecap Morello programs
  with ``clang``.

  - If you're using the LLVM Docker image, ``cocoaxu/morelloie-llvm``, then the 
    CHREI compiler is ``clang``, and we need to specify the target as 
    ``aarch64-linux-musl_purecap`` and the sysroot as ``/root/musl-sysroot-purecap``:

  .. code-block:: shell

    $ clang -march=morello \
      --target=aarch64-linux-musl_purecap \
      --sysroot=/root/musl-sysroot-purecap \
      test.c -o test -static


  - If you're using the GCC Docker image, ``cocoaxu/morelloie-gcc``, then we can compile
    the program with ``gcc`` and we don't need to specify the target or sysroot:

    .. code-block:: shell

      $ gcc -march=morello+c64 -mabi=purecap \
        test.c -o test -static


  After the compilation, you need to run the program with ``morelloie``. For example:

  .. code-block:: shell

    $ morelloie -- ./test


Native compilation
==================

If you have access to Morello hardware, you can run the LLVM compiler natively
on CheriBSD. You need to install llvm with pkg, then you can run it as you would on any other platform.

.. code-block:: bash

   pkg64 install llvm-morello

Then you can invoke the clang compiler to compile c programs
to purecap binaries like this:

.. code-block:: bash

   clang-morello -march=morello+c64 -mabi=purecap ./file.c

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

   $ clang-morello -march=morello+c64 -mabi=purecap ./varidiac.c
   $ ./a.out
   In-address space security exception (core dumped)


To fix this, we have to pass an extra argument, `-Xclang -morello-vararg=new`,
to clang to indicate that we intend to use varidiac functions with
capability pointers:

.. code-block:: bash

   $ clang-morello -march=morello+c64 -mabi=purecap -Xclang -morello-vararg=new ./varidiac.c
   $ ./a.out
   Total bytes: 24


Which compiler?
===============

The Cambridge CHERI team support the LLVM compiler. This can target various CHERI architectures including RISC-V and Morello. This is the default compiler for the CheriBSD OS.

The Arm Morello development team have a GCC compiler port.

