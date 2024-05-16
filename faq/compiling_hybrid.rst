========================================
 How do I compile for hybrid mode CHERI?
========================================

If you want to generate *hybrid* code for Morello,
you can use a CHERI LLVM compiler but you need to specify
the appropriate ABI flag.

Native compilation
==================

If you have access to Morello hardware, you can run the LLVM compiler natively
on CheriBSD. You need to install llvm with pkg, then you can run it as you would on any other platform.

.. code-block:: bash

   pkg64 install llvm-morello

Then you can invoke the clang compiler to compile C programs
to hybrid binaries like this:

.. code-block:: bash

   clang-morello -march=morello -mabi=aapcs ./file.c

You should specify both the march and mabi flags explicitly for
hybrid mode. AAPCS stands for "Arm Architecture Procedure Call Standard" which is the default AArch64 ABI.

Cross-compilation
=================

The LLVM compiler runs in cross-compilation mode for CHERI, so you can compile to CHERI targets from a non-CHERI platform like x86_64/linux.
If you use cheribuild, then by default your CHERI LLVM toolchain will be
located in the `cheri` directory in your home directory. There is a
configuration file with the relevant parameters set up to enable cross-compilation. One convenient way to
invoke the cross-compiler is by defining appropriate environment variables:

.. code-block:: bash

   export CC=~/cheri/output/morello-sdk/bin/clang
   export CFLAGS="--config cheribsd-morello-hybrid.cfg"

You can then invoke the cross-compiler on your non-CHERI platform:

.. code-block:: bash

   $CC $CFLAGS test.c

which will generate an `a.out` executable file that you can copy to a
CHERI system to execute.


Using a standard AArch64 compiler
=================================

If you compile C code natively on (say) FreeBSD and generate
AArch64 code, this should work on CheriBSD as a hybrid binary.
Of course, it's worth checking compatibility between compiler
ABIs, FreeBSD/CheriBSD versions and shared objects ... but
this might be a relatively straightforward way to generate
hybrid executables for CheriBSD.
