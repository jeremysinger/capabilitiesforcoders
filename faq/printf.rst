======================================
How do I printf a capability value?
======================================

The printf function has been extended with support for displaying
capability values.
Capability-aware printf re-uses the C format specifier for
pointer values, i.e. ``%p``.

You can print the raw address in the capability, or print out
the raw address plus metadata, or print out the raw bit-pattern of
the full capability. There are different variants of the format
specifier for each of these options.


.. code-block:: C
   :emphasize-lines: 2

   void *x = (void *)malloc(1);
   printf("The capability is %p\n", x);



