===========================
Capability misuse warning
===========================

If you see a CHERI Clang/LLVM warning message like this::

   warning: cast from provenance-free integer type to
   pointer type will give pointer that can not be dereferenced
   [-Wcheri-capability-misuse]

then you probably have tried to compile code that looks something like this:

.. code-block:: C
   :emphasize-lines: 3

   #include <stdint.h>

   void *foo(uint64_t value) {
     return (void *)value;
   }


The problem is that the plain integer value ``value``
does not have any capability metadata associated with it.
When we try to cast ``value`` to a pointer, the constructed
capability will have completely invalid metadata, so the
resulting capability pointer cannot be used in any meaningful
way.


Avoiding this warning
^^^^^^^^^^^^^^^^^^^^^

The best way to avoid the warning is to consider why your code
is storing pointer information in a non-pointer datatype.
This code must be refactored for CHERI. Pointer values should be
stored as ``uintptr_t`` types, or retained as pointers directly.

An example refactoring would be:

.. code-block:: C
   :emphasize-lines: 3

   #include <stdint.h>

   void *foo(uintptr_t value) {
     return (void *)value;
   }

which does not trigger any compiler warnings since the relevant
capability metadata is preserved in the ``uintptr_t`` datatype.
