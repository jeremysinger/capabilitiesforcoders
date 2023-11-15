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



.. code-block:: C
   :emphasize-lines: 28,32

   #include <cheriintrin.h>
   #include <sys/cdefs.h>
   #include <machine/sysarch.h>
   #include <stdio.h>
   #include <stdlib.h>
   #include <sys/sysctl.h>


   int main()
   {
       void *sealcap;
       size_t sealcap_size = sizeof(sealcap);
       if (sysctlbyname("security.cheri.sealcap", &sealcap, &sealcap_size, NULL, 0) < 0)
       {
	   printf("Fatal error. Cannot get `security.cheri.sealcap`.");
	   exit(1);
       }

       printf("---- sealcap ----\n");
       printf("sealcap: %#p\n", sealcap);

       void *buffer = malloc(64);
       printf("---- buffer (before sealing) ----\n");
       printf("buffer: %#p\n", buffer);

       buffer = cheri_seal(buffer, sealcap);
       printf("---- buffer (after sealing) ----\n");
       printf("sealed: %#p\n", buffer);

       buffer = cheri_unseal(buffer, sealcap);
       printf("---- buffer (after unsealing) ----\n");
       printf("unsealed: %#p\n", buffer);

       free(buffer);
       return 0;
   }



