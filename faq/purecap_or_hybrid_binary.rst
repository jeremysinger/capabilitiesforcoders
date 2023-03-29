==========================================================
How do I tell whether an executable is purecap or hybrid?
==========================================================

There are two modes of execution for CHERI: `purecap` and `hybrid`.
For a `purecap` executable, all memory references are represented as
capability values. On the other hand, for a `hybrid` executable,
memory references are represented as raw pointers, i.e. the code
is not capability-aware.

CheriBSD can run both purecap and hybrid binaries. Depending on how
your system is configured, the relevant libraries will be in different
directories. (For instance, for purecap CheriBSD, purecap libraries are in `/usr/lib` and hybrid libraries are in `/usr/lib64`.)

You can use the CheriBSD ``file`` command to detect whether a binary
file is `purecap` (known as `C64`) or `hybrid` (known as `A64`).

.. code-block:: bash

   file test-purecap.exe

The output will look like this
::
   
   ELF 64-bit LSB pie executable, ARM aarch64, C64,
   CheriABI, version 1 (SYSV), dynamically linked,
   interpreter /libexec/ld-elf.so.1, for FreeBSD 14.0
   (1400064), FreeBSD-style, with debug_info, not stripped


For a hybrid mode executable, the file command will show the
elf file as `A64`:

.. code-block:: bash

   file test-hybrid.exe

The output will look like this
::
   
   ELF 64-bit LSB executable, ARM aarch64, A64,
   version 1 (FreeBSD), dynamically linked,
   interpreter /libexec/ld-elf.so.1, for FreeBSD 14.0
   (1400064), FreeBSD-style, with debug_info, not stripped
