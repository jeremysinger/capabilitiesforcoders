===========================
How do I seal a capability?
===========================

One capability may be *sealed* with another capability,
using the ``cheri_seal`` CHERI macro.
The first parameter of ``cheri_seal`` is the capability to
be sealed. The second parameter is the capability we are using
to perform the seal, analogous to the private key in a cryptographic
signing operation.
The result of successful ``cheri_seal`` is a sealed capability.

The example code below uses the OS canonical sealing capability,
``security.cheri.sealcap`` which can be used to
seal any valid capability value.

Unsealing a capability
----------------------

Once a capability has been sealed, it cannot be
dereferenced or modified. Effectively a sealed
capability is immutable. The only valid operation
we can perform is to unseal the capability, using
the ``cheri_unseal`` macro.

This is the dual of the ``cheri_seal`` macro above.
With ``cheri_unseal(x, y)``, ``x`` is the sealed
capability we want to unseal, and ``y`` is the
sealing capability (the same one that we used to perform the
seal).


.. code-block:: c
  :emphasize-lines: 39,43

  // seal.c
  #include <stdio.h>
  #include <stdlib.h>
  #include <string.h>
  #include <cheriintrin.h>
  #include <sys/sysctl.h>

  char * get_secret() {
    char * buf = (char *)malloc(32);
    memset(buf, 0, 32);
    strcpy(buf, "Shh! This is a secret!");
    return buf;
  }

  void * get_system_sealer() {
    void * sealcap;
    size_t sealcap_size = sizeof(sealcap);
    if (sysctlbyname("security.cheri.sealcap", &sealcap, &sealcap_size, NULL, 0) < 0)
    {
        fprintf(stderr, "Fatal error. Cannot get `security.cheri.sealcap`.");
        exit(1);
    }
    return sealcap;
  }

  void untrusted_3rd_party_func(void * data) {
    // this function will only print metadata of the sealed capability
    // and that's safe
    printf("[untrusted func] sealed: %#p, valid: %d\n", data, cheri_is_valid(data));
  }

  int main() {
    char * buf = get_secret();
    printf("[main] buf: %#p, valid: %d\n", buf, cheri_is_valid(buf));

    // we can seal the capability using `cheri_seal`
    void * sealer = get_system_sealer();
    printf("[main] sealer: %#p, valid: %d\n", sealer, cheri_is_valid(sealer));
    void * sealed = cheri_seal(buf, sealer);
    printf("[main] sealed: %#p, valid: %d\n", sealed, cheri_is_valid(sealed));

    // we can unseal the capability with the original sealer
    char * unsealed = (char *)cheri_unseal(sealed, sealer);
    printf("[after] unsealed: %#p, %s\n", unsealed, unsealed);

    // pass the sealed capability to an untrusted function
    untrusted_3rd_party_func(sealed);
  }


Note that the example code above won't be able to compile with the MUSL libc library because 
MUSL libc does not have support for sysctl. Therefore, we have to either compile it natively
on CheriBSD or with the GCC toolchain. 

If we build and run the program, we will see the following output:

.. code-block:: bash

  # compile and run on a Morello system
  $ clang-morello -march=morello+c64 -mabi=purecap \
      -Xclang -morello-vararg=new \
      -O0 -g seal.c -o seal
  $ ./seal
  [main] buf: 0x40838000 [rwRW,0x40838000-0x40838020], valid: 1
  [main] sealer: 0x4 [,0x4-0x2000], valid: 1
  [main] sealed: 0x40838000 [rwRW,0x40838000-0x40838020] (sealed), valid: 1
  [after] unsealed: 0x40838000 [rwRW,0x40838000-0x40838020], Shh! This is a secret!
  [untrusted func] sealed: 0x40838000 [rwRW,0x40838000-0x40838020] (sealed), valid: 1


Now if we try read the sealed capability in the untrusted function, we'll receive a
`SIGPROT` fault:

.. code-block:: c
  :emphasize-lines: 3

  void untrusted_3rd_party_func(void * data) {
    printf("[untrusted func] sealed: %#p, valid: %d\n", data, cheri_is_valid(data));
    printf("[untrusted func] read as char *: %s\n", (char *)data);
  }


.. code-block:: bash
  :emphasize-lines: 7

  $ ./seal
  [main] buf: 0x40838000 [rwRW,0x40838000-0x40838020], valid: 1
  [main] sealer: 0x4 [,0x4-0x2000], valid: 1
  [main] sealed: 0x40838000 [rwRW,0x40838000-0x40838020] (sealed), valid: 1
  [after] unsealed: 0x40838000 [rwRW,0x40838000-0x40838020], Shh! This is a secret!
  [untrusted func] sealed: 0x40838000 [rwRW,0x40838000-0x40838020] (sealed), valid: 1
  In-address space security exception (core dumped)


However, since we were using the OS canonical in the above example, which is
accessible by everyone, the attacker can also get a copy of that and try to
unseal the capabilities we pass to these untrusted functions. For example:

.. code-block:: c
  :emphasize-lines: 6-7

  void untrusted_3rd_party_func(void * data) {
    printf("[untrusted func] sealed: %#p, valid: %d\n", data, cheri_is_valid(data));
    
    void * root_sealer;
    size_t sealcap_size = sizeof(root_sealer);
    sysctlbyname("security.cheri.sealcap", &root_sealer, &sealcap_size, NULL, 0);
    data = cheri_unseal(data, root_sealer);
    printf("[untrusted func] read as char *: %s\n", (char *)data);
  }


.. code-block:: bash
  :emphasize-lines: 7

  $ ./seal
  [main] buf: 0x40838000 [rwRW,0x40838000-0x40838020], valid: 1
  [main] sealer: 0x4 [,0x4-0x2000], valid: 1
  [main] sealed: 0x40838000 [rwRW,0x40838000-0x40838020] (sealed), valid: 1
  [after] unsealed: 0x40838000 [rwRW,0x40838000-0x40838020], Shh! This is a secret!
  [untrusted func] sealed: 0x40838000 [rwRW,0x40838000-0x40838020] (sealed), valid: 1
  [untrusted func] read as char *: Shh! This is a secret!


To address this potential security issue, we can create our own sealer, and seal sensetive
capabilities with it instead of the OS canonical one. The reason for doing so is that, in
CHERI, a sealed capability can only be unseal with its original sealer.

In order to create our own sealer, we can derive it from the the OS canonical one. But before
that, we can take a look at output for the OS canonical one, which serves as the userspace
root sealer.

.. code-block:: bash

  $ ./seal
  ...
  [main] sealer: 0x4 [,0x4-0x2000], valid: 1
  ...


As shown above, the address range of the OS canonical sealer is `[0x4, 0x2000)`, and its current
offset is `0x0`. Therefore, in this example, we can derive our own sealer by change the offset of
the root sealer:

.. code-block:: c
  :emphasize-lines: 5-6

  void * get_derived_sealer() {
    static void * sealer = NULL;
    if (!sealer) {
      void * root_sealer = get_system_sealer();
      size_t offset = arc4random() % cheri_length_get(root_sealer);
      sealer = cheri_offset_set(root_sealer, offset);
    }
    return sealer;
  }


And now we can seal the secret with the derived sealer in the main function:

.. code-block:: c
  :emphasize-lines: 6,24-25

  int main() {
    char * buf = get_secret();
    printf("[main] buf: %#p, valid: %d\n", buf, cheri_is_valid(buf));

    // we can seal the capability to prevent tampering using `cheri_seal`
    void * sealer = get_derived_sealer();
    printf("[main] sealer: %#p, valid: %d\n", sealer, cheri_is_valid(sealer));
    void * sealed = cheri_seal(buf, sealer);
    printf("[main] sealed: %#p, valid: %d\n", sealed, cheri_is_valid(sealed));

    // we can unseal the capability with the original sealer
    char * unsealed = (char *)cheri_unseal(sealed, sealer);
    printf("[after] unsealed: %#p, %s\n", unsealed, unsealed);

    // pass the sealed capability to an untrusted function
    untrusted_3rd_party_func(sealed);
  }

  void untrusted_3rd_party_func(void * data) {
    printf("[untrusted func] sealed: %#p, valid: %d\n", data, cheri_is_valid(data));
    
    void * root_sealer;
    size_t sealcap_size = sizeof(root_sealer);
    sysctlbyname("security.cheri.sealcap", &root_sealer, &sealcap_size, NULL, 0);
    data = cheri_unseal(data, root_sealer);
    printf("[untrusted func] read as char *: %s\n", (char *)data);
  }


If we build and run the program now, the untrusted function won't be able to use the
default root sealer to unseal the capability we passed to it, and result in a `SIGPROT`
fault:

.. code-block:: bash
  :emphasize-lines: 6,10

  $ clang-morello -march=morello+c64 -mabi=purecap \
      -Xclang -morello-vararg=new \
      seal.c -o seal
  $ ./seal
  [main] buf: 0x40838000 [rwRW,0x40838000-0x40838020], valid: 1
  [main] sealer: 0x170c [,0x4-0x2000], valid: 1
  [main] sealed: 0x40838000 [rwRW,0x40838000-0x40838020] (sealed), valid: 1
  [after] unsealed: 0x40838000 [rwRW,0x40838000-0x40838020], Shh! This is a secret!
  [untrusted func] sealed: 0x40838000 [rwRW,0x40838000-0x40838020] (sealed), valid: 1
  In-address space security exception (core dumped)
