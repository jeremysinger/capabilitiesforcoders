====================
Implicit conversion 
====================

In Hybrid Mode
--------------

When compiling code using hybrid ABI, if you see a CHERI
Clang/LLVM warning message like this::

   warning: converting non-capability type 'void *' to
   capability type 'void * __capability' without an explicit
   cast; if this is intended use __cheri_tocap
   [-Wcheri-implicit-conversion]

then you probably have tried to compile code that looks something like this:

.. code-block:: C
   :emphasize-lines: 5

   #include <stddef.h>
   #include <stdint.h>

   void * __capability foo(void * value, ptrdiff_t offset) {
     return value + offset;
   }



The problem is that you're returning a non-capability pointer 
``value + offset``, however, the function's return type says
the function will return a capability-enabled pointer. When we
try to cast ``value + offset`` to a capability-enabled pointer 
``void * __capability``, we need an explicit cast.

Avoiding this warning
^^^^^^^^^^^^^^^^^^^^^

The best way to avoid the error is to consider refactoring the 
code so that pointers passed around have capability metadata
wherever they can in the source code.

An example refactoring would be:

.. code-block:: C
   :emphasize-lines: 4

   #include <stddef.h>
   #include <stdint.h>

   void * __capability foo(void * __capability value, ptrdiff_t offset) {
     return value + offset;
   }

which preserves the capability metadata.

However, in cases where it's not possible to accept a capability-enanbled pointer,
you can do an explicit cast using ``__cheri_tocap``.

An example refactoring would be:

.. code-block:: C
   :emphasize-lines: 5

   #include <stddef.h>
   #include <stdint.h>

   void * __capability foo(void * value, ptrdiff_t offset) {
     return (__cheri_tocap void * __capability)(value + offset);
   }

which allows casting to capability-enanbled pointer.
