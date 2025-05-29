===============================================
How do I compare two capabilities for equality?
===============================================

The CHERI intrinsic ``int cheri_is_equal_exact(void *x, void *y)`` from the header file ``cheriintrin.h`` compares the pointer and the metadata values, and only returns `true` if the two capabilities are bit-equivalent in all corresponding 129 bits.

On the other hand, the simple C ``==`` operator 
only compares the 64 bits of the
pointer value, without any inspection of metadata.
This means ``==`` is good for pointer equality checks, but
not appropriate for capability equality, so select your
equality relation with care!





