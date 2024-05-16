===============================================
How do I set the bounds for a capability value?
===============================================

We can explicitly restrict the bounds of
an existing capability by calling the
``void *cheri_bounds_set(void *capability, size_t bounds)``
macro function, available in the ``<cheriintrin.h>`` header.

This macro takes a *more permissive* capability (one with
larger bounds) and a bound (specified in bytes). It
derives a *more restricted* capability with narrower bounds,
which is the return value of the `cheri_bounds_set` call.

Note that hardware-checked bounds are a key feature of
CHERI capabilities.
Every capability comes with bounds, which are encoded
in the metadata, and will have been set by default by the
source of the capability, perhaps ``malloc`` or ``mmap``.
User code application code may set tighter bounds on
capabilities. User code *cannot* set looser bounds, since this
would break the capability monotonicity property (see below).

Here is some example code, where we have a more permissive pointer
`p` and a less permissive version `q`, with the same base but
a smaller bound.

.. code-block:: C
   :emphasize-lines: 13

   #include <cheriintrin.h>
   #include <stdio.h>
   #include <stdlib.h>
   
   int main() {
       int *p = malloc(5*sizeof(int));
       int *q = NULL;
       if (p==NULL) {
           fprintf(stderr, "out of memory\n");
           exit(EXIT_FAILURE);
       }
       p[4] = 42;
       q = cheri_bounds_set(p, 3*sizeof(int));
       q[2] = 42;
       // can't access q[3] or q[4] !!
       return 0;
   }



What happens if we try to set a more permissive bound?
======================================================

The monotonicity property of CHERI means we cannot set
the bounds of a derived capability to be more permissive
than the original capability.

If we call `cheri_bounds_set` and attempt to derive
a more permissive capability, then the capability loses
its validity tag, so if it is used in a dereferencing operation
it will cause a SIGPROT error.

The gdb single-step execution below illustrates this point:

.. code-block:: C
   :emphasize-lines: 2,8

   (gdb) step
   13	  q = cheri_bounds_set(p, 10*sizeof(int));
   (gdb) print p
   $2 = (int *) 0x40c0f000 [rwRW,0x40c0f000-0x40c0f014]
   (gdb) step
   ...
   (gdb) print q
   $3 = (int *) 0x40c0f000 [rwRW,0x40c0f000-0x40c0f028] (invalid)
