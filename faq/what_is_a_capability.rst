============================
What is a CHERI capability?
============================

A capability is an unforgeable token of authority that grants particular rights to perform an action in an execution context. 

In the CHERI context, a capability is a `fat pointer` that includes
not only a raw memory address, but also associated metadata for access permissions,
bounds and pointer validity.

CHERI stands for `Capability Hardware Enhanced RISC Instructions`:
this is a bolt-on to a standard RISC instruction set architecture to
provide support for capabilities in the processor architecture.
The prototype `Arm Morello <https://www.arm.com/architecture/cpu/morello>`_ processor supports capabilities natively
in hardware.
