==============================================================
Which header file(s) should I include to use CHERI functions?
==============================================================

To call CHERI API functions from your own C code,
include the ``cheriintrin.h`` header file. This will be available
as a system header file with CHERI C compiler toolchains.
In general, CHERI API functions start with the ``cheri_`` prefix.

.. code-block:: C
   :emphasize-lines: 1

   #include <cheriintrin.h>
   int main() {
     void *x = (void *)malloc(1);
     printf("The bounds of this capability are %d\n",
            cheri_length_get(x));
     return 0;
   }


