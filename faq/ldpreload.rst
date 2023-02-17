========================================================
How do I use the LD_PRELOAD trick for CheriBSD?
========================================================

In common with other Unix variants, you can set the ``LD_PRELOAD``
environment variable to point to a specific, single shared object file
that will be loaded before any other shared objects. This allows
you to override functions in the platform libraries by creating a
custom library with the same symbols and specifying
the absolute path to your library in ``LD_PRELOAD``.
This works for *purecap* binaries with *purecap* shared objects.

For *hybrid* binaries running with *hybrid* shared objects,
you need to use the ``LD_64_PRELOAD`` environment
variable instead.


In Bash, 

.. code-block:: bash

   export LD_PRELOAD=/path/to/customlib.so
   /run/my/binary
   # or for hybrid code ...
   export LD_64_PRELOAD=/path/to/customlib.so

or for C shell:

.. code-block:: csh

   setenv LD_PRELOAD /path/to/customlib.so
   /run/my/binary
   # or for hybrid code ...
   setenv LD_64_PRELOAD /path/to/shared/object/files

Note you can use the `ldd` command with a binary
to see precisely which shared objects will be loaded.



