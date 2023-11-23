=========================================
What is a CHERI capability? (long answer)
=========================================

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

For example, let's say that we have the following ``func.c`` program:

.. code-block:: C

   // func.c
   #include <stdio.h>

   void func() { }

   int main(int argc, char *argv[])
   {
      void *cap1 = func;
      void *cap2 = cap1 + 1;
      printf("%#p\n", cap1);
      printf("%#p\n", cap2);
   }


We'll mention the ``%#p`` format specifier later once we saw the output.

You can follow the example code above if you have a Morello board or use MorelloIE, the Morello 
Instruction Emulator. You can use these non-official prebuilt MorelloIE Docker images, such
as ``cocoaxu/morelloie-llvm`` and ``cocoaxu/morelloie-gcc`` for a quick start. They contain
LLVM-toochain and GCC-toolchain respectively. Please refer to the post,
`How do I compile for CHERI? <https://capabilitiesforcoders.com/faq/compiling.html>`_ for more
information about how to set up and how to compile for CHERI.

Now we can compile it with the CHERI compiler, and based on your enviroment,
the compilation command varies:

- If you're using the LLVM Docker image, ``cocoaxu/morelloie-llvm``, then the 
  CHREI compiler is ``clang``, and we need to specify the target as 
  ``aarch64-linux-musl_purecap`` and the sysroot as ``/root/musl-sysroot-purecap``:

  .. code-block:: shell

   $ clang -march=morello \
       --target=aarch64-linux-musl_purecap \
       --sysroot=/root/musl-sysroot-purecap \
       -O0 -g func.c -o func -static


- If you're using the GCC Docker image, ``cocoaxu/morelloie-gcc``, then we can compile
  the program with ``gcc`` and we don't need to specify the target or sysroot:

  .. code-block:: shell

   $ gcc -march=morello+c64 -mabi=purecap \
       -O0 -g func.c -o func -static


- If you're on a Morello system, then you can compile the program with ``clang-morello``;
  also, you don't need to specify the target or sysroot. However, for old versions of 
  ``clang-morello`` you need to add the ``-Xclang -morello-vararg=new`` flag to the 
  compilation command to enable the new vararg ABI for Morello:

  .. code-block:: shell

   $ clang-morello -march=morello+c64 -mabi=purecap \
       -Xclang -morello-vararg=new \
       -O0 -g func.c -o func


After the compilation, if you're on a Morello system, you can simply execute the program
as what you'd do on a normal aarch64 Linux system without the need of ``morelloie`` for
emulating the CPU. 

If you're using either the LLVM or the GCC MorelloIE Docker image, you need to run the
program with ``morelloie``. The output of the program is shown below. (In the following
sections, we'll only show the output of ``morelloie``, as the output of the example program
on a Morello system should be the same, except for the memory address due to ASLR 
(Address space layout randomization).)

.. code-block:: shell

   $ morelloie -- ./func
   0x211545 [rxRE,0x200200-0x226c40] (sentry)
   0x211546 [rxRE,0x200200-0x226c40] (invalid,sentry)

   $ ./func
   0x110a3d [rxR,0x100000-0x130e80] (sentry)
   0x110a3e [rxR,0x100000-0x130e80] (invalid,sentry)


The ``%#p`` format specifier is used to print a capability pointer in hexadecimal format along with some
properties and attributes of this capability pointer. It prints the capability pointer in the following
format:

.. code-block:: text

   <address> [<permissions>,<base>-<top>] (<attr>)


which contains its address (``0x211545``), permissions (``rxRE``),  and the semi-closed memory range
(``[base, limit)``) that the capability points to (``0x200200-0x226c40``). And the attributes of the 
capability pointer like the ``sentry`` keyword at the end of the output.

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
