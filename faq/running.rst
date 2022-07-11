============================
How do I run CHERI programs?
============================

Emulation
---------

You can use a full-system software  emulator, running CheriBSD.
Use the cheribuild utility to build a QEMU executable for
Morello:

.. code-block:: bash

   python3 cheribuild.py run-morello-purecap

This target builds QEMU and drops you straight into the running CheriBSD
instance at the root login prompt. By default, the root password is blank
so just press enter to login.

To quit QEMU, press CTRL+A then CTRL+X.



Native execution
----------------

The most common native platform for CHERI is the Arm Morello system, running
CheriBSD.

Other targets include FPGA instantiations of CHERI RISC-V.
