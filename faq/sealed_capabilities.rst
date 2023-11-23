===========================
What is a Sealed Capability
===========================

This post serves as the second part of the previous post `What is a Capability <https://capabilitiesforcoders.com/faq/capabilities.html>`_,
and in this post, we will talk about what a sealed capability is in CHERI. Also,
we will explain sealed capabilities using the example program in the previous post,
so if you haven't read the previous post, please read it first.

Before we dive into the details of these capabilities in the ``func`` program in the previous post,
let's talk about what a sealed capability is. In CHERI and Morello, a capability can be sealed,
and in simple words, a sealed capability is a capability with a non-zero object type.

The object type is a 16-bit field in Morello, and there're 4 special values for the object type:

- ``0x0000``: the capability is not sealed.
- ``0x0001``: the capability is RB-sealed and used for all conventional register branch.
- ``0x0002``: the capability is LPB-sealed, which is used for load pair and branch operations (relevant to compartments).
- ``0x0003``: the capability is LB-sealed and used for load and branch operations (relevant to compartments).

The RB-, LPB- and LB-sealed capabilities are also referred to as "fixed" or "hardware" types.

There are 4 consequences for sealing a capability:

- Once the capability is sealed, it will be immutable. Any operations that modify the capability
  will result in an invalid capability.
- A sealed capability cannot be dereferenced, that is, we cannot read or write the memory that
  the capability points to.
- Also, branching to an executable but sealed capability will fault. Notice RB-, LPB- and LB-sealed
  capabilities will be automatically unsealed during the corresponding branch operation.
- Lastly, a sealed capability cannot be used to seal another capability even when meeting all other
  requirements for a sealing capability.

There are also other values for the object type, which are used for sealing capabilities, but we
will not cover them here. 

RB-sealed Capabilities
----------------------

If we break at the ``printf`` function in the example above and as ``cap1`` is the second parameter 
for ``printf``, it's stored in the register ``c1``. Hence we can do ``p c1`` to inspect the first 
capability, ``cap1``, in the debugger:

.. code-block:: shell

    $ morelloie -break printf -- ./func
    /* next instruction (211c18:printf) */
    /* 211c18 0280c3ff sub     csp, csp, #48, lsl #0 */
    [281:211c18] p c1
    c1 = 0x1:b090c000:8d9f0044:00000000:00211545
              tag: true
          address: 0x00000000000211545
             base: 0x00000000000200200
            limit: 0x00000000000226cc0
           bounds: valid
        in bounds: true
           length: 158400
           offset: 70469
      permissions: GrRM---xES--------
           sealed: sealed RB (1)
    ...
    [281:211bec] c
    0x211545 [rxRE,0x200200-0x226c80] (sentry)


The output shows that if we take the address of a function, it will result in a RB-sealed
capability (``sealed RB (1)``). And the sentry keyword outputted by printf also suggested 
that what we see is an executable capability that is sealed with the RB object type.

The second capability ``cap2`` is invalid because it was created by adding 1 to the first
capability ``cap1``, and once we do any arithmetic operations on a sealed capability, the
resulting capability will be invalid, as shown in the output above (``(sentry, invalid)``).
If we try to print the second capability ``cap2`` in the debugger, we will get the following
output:

.. code-block:: shell

    $ morelloie -break printf -- ./func
    ...
    [293:211bc8] p c1
    c1 = 0x1:dc104000:5f40df30:0000ffff:f063df30
              tag: true
          address: 0x00000fffff063df30
             base: 0x00000fffff063df30
            limit: 0x00000fffff063df40
           bounds: valid
        in bounds: true
           length: 16
           offset: 0
      permissions: GrRMwWL-----------
           sealed: (not sealed)
    ...
    [293:211bc8] c
    0x211546 [rxRE,0x200200-0x226c40] (invalid,sentry)


Notice that the second capability ``cap2`` is not sealed anymore, and its permissions have
also changed. It's no longer executable, meaning that we cannot jump right in the middle
of the function ``func``. In this way, we can 

Another example is the capability holding return address (link register), which is the register
``c30``. If we break at the ``printf`` function and inspect the capability holding the return
address, we will get the following output:

.. code-block:: shell

    $  morelloie -break printf -- ./func
    /* next instruction (211bc8:printf) */
    /* 211bc8 0280c3ff sub     csp, csp, #48, lsl #0 */
    [294:211bc8] p clr
    clr = 0x1:b090c000:8d8f0044:00000000:0021159d
              tag: true
          address: 0x0000000000021159d
             base: 0x00000000000200200
            limit: 0x00000000000226c40
           bounds: valid
        in bounds: true
           length: 158272
           offset: 70557
      permissions: GrRM---xES--------
           sealed: sealed RB (1)
    ...


The output shows that the return address is also a RB-sealed capability. This means that
the return address is also protected by the sentry. If we try to modify the return address,
the program will crash. And this is how Morello protects the control flow of a program.

LPB- and LB-sealed Capabilities
-------------------------------

For LPB- and LB-sealed capabilities, they are used for load pair and branch operations. In
order to create a LPB- or LB-sealed capability, we need to use inline assembly for this:

.. code-block:: C

    inline __attribute__ ((naked))
    void *__morello_seal_lpb(void *cap)
    {
        void *ret;
        __asm__ ("seal %0, %1, lpb" : "=C"(ret) : "C"(cap));
        return ret;
    }

    inline __attribute__ ((naked))
    void *__morello_seal_lb(void *cap)
    {
        void *ret;
        __asm__ ("seal %0, %1, lb" : "=C"(ret) : "C"(cap));
        return ret;
    }


The ``seal`` instruction is used to seal a capability. The first operand is the destination
register, and the second operand is the source register. The third operand is the sealing
type, which can be ``lpb`` or ``lb``. These sealing types are used for sealing a capability 
with the LPB and LB object types, respectively.

You can read the `["Hello World" Example] <https://capabilitiesforcoders.com/compartmentalisation/index.html#morello-compartmentalisation>`_
in the Morello Compartmentalisation section. It shows how to use LPB- and LB-sealed capabilities
to compartmentalise a program.
