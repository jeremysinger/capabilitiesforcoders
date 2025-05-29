======================================================
How do I access the auxiliary info vector in CheriBSD?
======================================================

What is the auxiliary information vector (auxvec) for?
What information does it contain?
`man aux` on CheriBSD shell - will give you some details
of fields in the auxiliary vector, but not all of them!

For more detail, need to look at header file:


How do we acces it?
===================

Simplest way - getauxvec... but some fields can't be
accessed like this - they seem to be zeroed out...

Use the `__auxvec` global variable
==================================

Alternative, to get the permissive capabilities
with extensive bounds, from the runtime loader...
look at the __auxvec global variable - see below...



Can't use etext, edata, - these are addresses, but don't have
bounds that we can scan across..

Works on clang-morello on CheriBSD (version) - but doesn't work
with cross-compiler...

.. code-block:: C
   :emphasize-lines: 13

   #include <stdio.h>
   #include <stdlib.h>
   #include <stdint.h>
   #include <sys/auxv.h>   // for elf_aux_info
   #include <errno.h>      // for errno
   #include <string.h>     // for strerror
   #include <elf.h>       // for elfinfo
   
   extern void *__auxargs;
   
   int main(void) {
       int ncpus;
       int result;
       uintptr_t entry;
   
       /* // AT_NCPUS is FreeBSD-specific and gives the number of CPUs available */
       /* result = elf_aux_info(AT_NCPUS, &ncpus, sizeof(ncpus)); */
       /* if (result != 0) { */
       /*     fprintf(stderr, "elf_aux_info failed: %s\n", strerror(errno)); */
       /*     return EXIT_FAILURE; */
       /* } */
   
       /* printf("Number of CPUs: %d\n", ncpus); */
   
       // AT_ENTRY is CHERI BSD specific and gives entrypoint to
       // process address space - see https://github.com/CTSRD-CHERI/cheri-exercises/blob/master/src/exercises/cheriabi/cheriabi.pptx
       
       /* result = elf_aux_info(AT_PHDR, &entry, sizeof(entry)); */
       /* if (result != 0) { */
       /*     fprintf(stderr, "elf_aux_info failed: %s\n", strerror(errno)); */
       /*     return EXIT_FAILURE; */
       /* } */
   
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
 - to scan data section for root set pointers,
in a garbage collecotr.- like BoehmGC...

