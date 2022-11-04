=============================
 How do I compile for CHERI?
=============================



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

   clang-morello -march=morello+c64 -mabi=purecap ./file.c

You need to specify both the march and mabi flags explicitly for
purecap mode.



Which compiler?
===============

The Cambridge CHERI team support the LLVM compiler. This can target various CHERI architectures including RISC-V and Morello. This is the default compiler for the CheriBSD OS.

The Arm Morello development team have a GCC compiler port.

