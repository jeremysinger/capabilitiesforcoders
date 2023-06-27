===================
Capability misuse 
===================

In Purecap Mode
---------------

When compiling code using purecap ABI, if you see a CHERI
Clang/LLVM warning message like this::

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

In Hybrid Mode
--------------

When compiling code using purecap ABI, if you see a CHERI
Clang/LLVM warning message like this::

   error: cast from capability type 'void * __capability' to
   non-capability type 'void *' is most likely an error;
   use __cheri_fromcap to convert between pointers and
   capabilities [-Wcheri-capability-misuse]

then you probably have tried to compile code that looks something like this:

.. code-block:: C
   :emphasize-lines: 4

   #include <stddef.h>
   #include <stdint.h>

   void *foo(void * __capability value, ptrdiff_t offset) {
     return (void *)(value + offset);
   }


The problem is that the pointer ``value`` has capability 
metadata associated with it. When we try to cast ``value + offset``
to a non-capability pointer ``void *``, the capability 
metadata associated in ``value`` will be lost.

Avoiding this error
^^^^^^^^^^^^^^^^^^^

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

However, in cases where it's not possible to return a capability-enanbled pointer,
you can do an explicit cast using ``__cheri_fromcap``.

An example refactoring would be:

.. code-block:: C
   :emphasize-lines: 4

   #include <stddef.h>
   #include <stdint.h>

   void * foo(void * __capability value, ptrdiff_t offset) {
     return (__cheri_fromcap void *)(value + offset);
   }

which allows to cast from capability-enanbled pointer to a plain one.
