======================================
How do I printf a capability value?
======================================

There is a new format specifier for capability values for printf, ``%p``

.. code-block:: C
   :emphasize-lines: 2

   void *x = (void *)malloc(1);
   printf("The capability is %p\n", x);



