=================================
Can I run Python code on Morello?
=================================

If you are running in hybrid mode, you can install the
default CPython interpreter with:

.. code-block:: bash

   pkg64 install python3

This works fine, but does not have runtime support for capabilities or
fine-grained memory protection.


Building MicroPython
--------------------

MicroPython is a cut-down Python interpreter, designed for embedded
systems. It has a smaller range of libraries than the default CPython.
There is a fully functional port of MicroPython to purecap on CheriBSD.
Below are the instructions for building MicroPython natively on
the CheriBSD Morello platform.

.. code-block:: bash

   # download the source code from github
   git clone https://github.com/glasgowPLI/micropython
   cd micropython
   git fetch origin morello-dev
   git switch morello-dev
   cd ports/unix

   # install dependencies
   pkg64 install git gmake llvm-morello python3 pkgconf
   pkg64c install libffi

   # now build micropython
   gmake CC='clang-morello -march=morello -mabi=purecap' CXX='clang++-morello -march=morello+c64 -mabi=purecap' LIBFFI_LDFLAGS='-L/usr/local/lib -lffi' submodules
   gmake CC='clang-morello -march=morello -mabi=purecap' CXX='clang++-morello -march=morello+c64 -mabi=purecap' LIBFFI_LDFLAGS='-L/usr/local/lib -lffi'

   # the binary is at ./build-standard/micropython


Using MicroPython
-----------------

The purecap MicroPython build on Morello supports tight
bounds for allocations, as demonstrated by this unsafe
code below, which works in vanilla MicroPython but
causes a capability protection fault on Morello.

.. code-block:: python

   import uctypes as uct
   tiny = bytearray(1)
   ptr = uct.addressof(tiny)
   # create a new bytearray based on ptr
   unsafe = uct.bytearray_at(ptr, 16384)
   # access out-of-bounds memory
   unsafe [200] = 0x1f

Further references
------------------

* Paper about `MicroPython port to CHERI <https://eprints.gla.ac.uk/304475/>`_ (published at ACM SIGPLAN MPLR 2023 conference)
* Paper about `optimizing MicroPython port to CHERI <https://eprints.gla.ac.uk/305684/>`_ (published at ACM SIGPLAN VMIL 2023 workshop)
* Documentation for `MicroPython project <https://docs.micropython.org/en/latest/>`_ (the most relevant docs are for the Unix port))
