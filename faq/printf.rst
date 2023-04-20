======================================
How do I printf a capability value?
======================================

The ``printf`` function has been extended with support for displaying
capability values.
Capability-aware ``printf`` re-uses the C format specifier for
pointer values, i.e. ``%p``.

You can print the raw address in the capability with the ``%p`` specifier, or print the address plus pretty-printed metadata with the ``%#p`` variant.


.. code-block:: C
   :emphasize-lines: 6-7

   #include <stdio.h>
   #include <stdlib.h>

   int main() {
     void *x = (void *)malloc(1);
     printf("The address in this capability is %p\n", x);
     printf("The full capability (incl metadata) is %#p\n", x);
   }

This will output:
::
   
   The address in this capability is 0x40823010
   The full capability (incl metadata) is 0x40823010 [rwRW,0x40823010-0x40823011]



More info
---------

Check out the CHERI C/C++ programming wiki info on
`printf formats <https://github.com/CTSRD-CHERI/cheri-c-programming/wiki/Displaying-Capabilities#printf3-formats>`_.
