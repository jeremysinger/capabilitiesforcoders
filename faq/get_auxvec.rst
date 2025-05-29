======================================================
How do I access the auxiliary info vector in CheriBSD?
======================================================

What is the auxiliary information vector (auxvec) for?
What information does it contain?
On Unix systems like CheriBSD, the auxiliary vector (auxvec)
passes runtime metadata from the kernel to the process
when it starts up.

Running ``man auxv`` on a CheriBSD shell will give you some details
about fields in the auxiliary vector, but this information is not
exhaustive!

For more detail, we need to look at relevant OS header files.


How do we access the auxvec?
============================

The simplest way is to use the ``getauxval`` routine
from ``<sys/auxv.h>`` as a key/value query function
but some fields can't be
accessed like this; they seem to be zeroed out.

.. code-block:: C
   :emphasize-lines: 14

   #include <stdio.h>
   #include <stdlib.h>
   #include <stdint.h>
   #include <sys/auxv.h>   // for elf_aux_info
   #include <errno.h>      // for errno
   #include <string.h>     // for strerror
   #include <elf.h>       // for elfinfo
   
   int main(void) {
       int ncpus;
       int result;
   
       // AT_NCPUS is FreeBSD-specific and gives the number of CPUs available
       result = elf_aux_info(AT_NCPUS, &ncpus, sizeof(ncpus));
       if (result != 0) {
           printf(stderr, "elf_aux_info failed: %s\n", strerror(errno));
           return EXIT_FAILURE;
       }
   
       printf("Number of CPUs: %d\n", ncpus);
       
       return EXIT_SUCCESS;
   }



Use the `__auxvec` global variable
==================================

Alternatively, we can make use of the
``__auxvec`` global variable directly.

Note the code below Works with clang-morello
running natively on CheriBSD, but it doesn't work
with a cross-compiler.

.. code-block:: C
   :emphasize-lines: 14

   #include <stdio.h>
   #include <stdlib.h>
   #include <stdint.h>
   #include <sys/auxv.h>   // for elf_aux_info
   #include <errno.h>      // for errno
   #include <string.h>     // for strerror
   #include <elf.h>       // for elfinfo
   
   extern void *__auxargs;
   
   int main(void) {
       printf("Aux pointer: %#p\n", (void *)__auxargs);
   
       for (Elf_Auxinfo *auxp = __auxargs; auxp->a_type != AT_NULL; auxp++) {
         if (auxp->a_type == AT_ARGV) {
           printf("argv is %#p\n", (char **)auxp->a_un.a_ptr);
         } else if (auxp->a_type == AT_ARGC) {
           printf("argc is %d\n", (int)auxp->a_un.a_val);
         } else if (auxp->a_type == AT_ENTRY) {
           printf("entry is %#p\n", (void *)auxp->a_un.a_ptr);
         } else if (auxp->a_type == AT_PHDR) {
           printf("phdr is %#p\n", (void *)auxp->a_un.a_ptr);
         }
       }
       return EXIT_SUCCESS;
   }



   
Use case
========

The PHDR value is a useful capability for scanning ELF `.data` sections for root set pointers, in a garbage collection.

