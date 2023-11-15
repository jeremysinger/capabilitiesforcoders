What is a Sealed Capability
===========================

Capability
----------

A capability is a token of authority over a continuous memory space. It associates traditional
pointers with a set of permissions and properties. Capabilities are unforgeable and unguessable,
and can be passed around between threads and processes. A capability have the following properties: 
`[1] <https://www.morello-project.org/resources/morello-linux-morelloie/#:~:text=Capability%20and%20its%20properties>`_

- Value: memory location address (the conventional pointer address).
- Tag: indicates if a capability is valid (and whether it can be dereferenced, i.e., used as a base address in a memory reference).
- Bounds: base, limit, validity (the latter shows whether current combination of base, value and limit are correctly encoded). Bounds make up a semi-closed interval where base is included, and limit is not.
- Global / Local: a property that shows whether a capability is global or local (ha-ha, see below though).
- Permissions: they control operations that are available via this capability (see below).
- Executable / System access / Executive: bits that define and describe executability of a capability.
- Object type: a property related to sealing of capabilities (see below).
- Flags: application defined bits.

You can follow the example code below if you have a Morello board or MorelloIE, the Morello 
Instruction Emulator. You can use the prebuilt MorelloIE Docker image, ``cocoaxu/morelloie``, 
for a quick start.


.. code-block:: shell

    $ docker run -it --rm cocoaxu/morelloie


Inside this Docker image, ``morelloie`` and ``clang`` are available. Besides that, a sysroot 
directory for Purecap can be found at ``/root/musl-sysroot-purecap``, which can be used later
when compiling Purecap Morello programs with clang. 

After compiling the example code, you can use ``morelloie`` to run your program. For example, 
let's say that we have the following ``func.c`` program:


.. code-block:: C

    // func.c
    #include <stdio.h>

    void func() { }

    int main(int argc, char *argv[])
    {
        void *cap1 = fun;
        void *cap2 = cap1 + 1;
        printf("%#p\n", cap1);
        printf("%#p\n", cap2);
    }


We'll mention the ``%#p`` format specifier later once we saw the output. Now we can 
compile it with ``clang`` using the following command, where we specify the target
as ``aarch64-linux-musl_purecap`` and the sysroot as ``/root/musl-sysroot-purecap``:

.. code-block:: shell

    $ clang -march=morello \
        --target=aarch64-linux-musl_purecap \
        --sysroot=/root/musl-sysroot-purecap \
        func.c -o func -static


To run the program, we can use ``morelloie``:

.. code-block:: shell

    $ morelloie -- ./func
    0x211545 [rxRE,0x200200-0x226c40] (sentry)
    0x211546 [rxRE,0x200200-0x226c40] (invalid,sentry)


The ``%#p`` format specifier is used to print a capability pointer in hexadecimal format along with some
properties of this capability pointer, which contains its address (``0x211545``), permissions (``rxRE``), 
and the semi-closed memory range (``[base, limit)``) that the capability points to (``0x200200-0x226c40``).
And some extra information like the ``sentry`` keyword at the end of the output.

The permissions part of a capability determine what operations are allowed via this capability, and they can
be grouped into the following categories: `[2] <https://www.morello-project.org/resources/morello-linux-morelloie/#:~:text=Permissions%20of%20a%20capability%20determine%20what%20operations%20are%20allowed%20via%20this%20capability.%20In%20a%20nutshell%2C%20they%20can%20be%20grouped%20into%20the%20following%20categories>`_

- data access (reading from and writing to memory),
- permission-like bits that aren't permissions per se but act like they in terms of monotonicity,
- code execution (this includes loading code from memory as well),
- custom permissions for specific use cases (such as compartment switches).

So the first line of the output shows that the first capability ``cap1`` points to the function ``func`` and 
the second capability ``cap2`` points to the next byte after the function ``func``. The permissions of the
first capability ``cap1`` is ``rxRE``, which means that it is readable, executable, and has the ``RE`` property. 
The ``RE`` property means that the capability is sealed, which means that it cannot be modified.

The ``sentry`` property means that the capability is a sentry capability, which means that it is a capability 
that is used to protect the memory space of the program. The ``sentry`` property is only available in Purecap 
Morello.

Usually, we don't need to explicitly change the permissions of a capability because the compiler and other
runtime library will do it. However, if you're in a situation where you need to have finer grained control over
the permissions of a capability, you can do that with builtin functions provided in ``cheriintrin.h``. For example,

.. code-block:: C

    #include <cheriintrin.h>
    #include <stdio.h>
    #include <stdlib.h>

    #define LOAD __CHERI_CAP_PERMISSION_PERMIT_LOAD__
    #define LOAD_CAP __CHERI_CAP_PERMISSION_PERMIT_LOAD_CAPABILITY__
    #define MUTABLE_LOAD __ARM_CAP_PERMISSION_MUTABLE_LOAD__

    #define STORE __CHERI_CAP_PERMISSION_PERMIT_STORE__
    #define STORE_CAP __CHERI_CAP_PERMISSION_PERMIT_STORE_CAPABILITY__
    #define STORE_LOCAL __CHERI_CAP_PERMISSION_PERMIT_STORE_LOCAL__

    void untrusted_3rd_party_func(char *str);

    int main()
    {
        char *str = malloc(sizeof(char) * 32);
        char *ro_str = cheri_perms_and(str, LOAD | LOAD_CAP | MUTABLE_LOAD);
        printf("str in main   : %#p\n", str);
        printf("ro_str in main: %#p\n", str);
        untrusted_3rd_party_func(ro_str);
    }

    void untrusted_3rd_party_func(char *str)
    {
        printf("str in func   : %#p\n", str);
        str[0] = 'A';
    }


In the example above, we have some macros that are used to define the permissions of a capability: 
`[3] <https://www.morello-project.org/resources/morello-linux-morelloie/#:~:text=Data%20access%20permissions%20cover%20reading%20and%20writing%20memory%20operations%20and%20include>`_

- LOAD: read data (non-capability) from memory,
- LOAD_CAP: read a capability from memory,
- MUTABLE_LOAD: read a capability with permissions allowing mutable operations,
- STORE: write data (non-capability) to memory,
- STORE_CAP: write a capability,
- STORE_LOCAL: write a local capability (without this permission in the memory reference a local capability cannot be stored).

As we want to make sure that the ``untrusted_3rd_party_func`` cannot modify the string, we use the builtin function
``cheri_perms_and`` to create a new capability ``ro_str`` that only has the ``LOAD``, ``LOAD_CAP`` and ``MUTABLE_LOAD``
permissions. The ``LOAD`` permission means that the capability can be used to load data from memory. As for the 
``LOAD_CAP`` and ``MUTABLE_LOAD`` permissions, they're used when we want to control access via copies of the 
capabilities that are shared with other components of our program.


If we compile the example above and run it with ``morelloie``, we will get the following output:

.. code-block:: shell

    $ clang -march=morello \
        --target=aarch64-linux-musl_purecap \
        --sysroot=/root/musl-sysroot-purecap \
        str.c -o str -static
    $ morelloie -- ./str
    str in main   : 0xffff80b98040 [rwRW,0xffff80b98040-0xffff80b98060]
    ro_str in main: 0xffff80b98040 [rwRW,0xffff80b98040-0xffff80b98060]
    str in func   : 0xffff80b98040 [rR,0xffff80b98040-0xffff80b98060]
    [emulator] simulated capability fault at 2116b4 in thread 402
    Insufficient permissions (required ----w-------------)
    0x1:90100000:40608040:0000ffff:80b98040
              tag: true
          address: 0x00000ffff80b98040
             base: 0x00000ffff80b98040
            limit: 0x00000ffff80b98060
           bounds: valid
        in bounds: true
           length: 32
           offset: 0
      permissions: -rRM--------------
           sealed: (not sealed)
            flags: 0
         exponent: 0
              top: 0x8060
           bottom: 0x8040
            local: true
    Segmentation fault


As we can see, the capability ``ro_str`` has the ``rR`` permissions, which means that it can be used to read data,
but cannot be used to write data. And when we try to modify the string in the ``untrusted_3rd_party_func``, the
Mollore Instruction Emulator will raise a capability fault with a hint telling us that the capability ``ro_str``
does not have the ``w`` permission, which is necessay for writing data to memory.

Sealed Capabilities
-------------------

Before we dive into the details of these capabilities in the ``func`` program above, let's talk 
about what a sealed capability is. In CHERI and Morello, a capability can be sealed, and in simple 
words, a sealed capability is a capability with a non-zero object type.

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
