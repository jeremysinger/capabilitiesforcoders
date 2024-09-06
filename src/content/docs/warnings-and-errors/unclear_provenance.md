---
title: Unclear provenance warning
description: Unclear provenance warning
---

If you see a CHERI Clang/LLVM warning message like this:

    binary expression on capability types 'uintptr_t'
    (aka 'unsigned __intcap') and 'uintptr_t'; it is
    not clear which should be used as the source of
    provenance; currently provenance is inherited
    from the left-hand side [-Wcheri-provenance]

then you probably have tried to compile code that looks something like
this:

```c {5}
#include <stdint.h>

uintptr_t combine(uintptr_t a, uintptr_t b) {
  uintptr_t c;
  c = (a&b);
  return c;
}
```

The problem is that capability values `a` and `b` both have their own
metadata, including bounds and validity tag bits. It is not clear how to
combine these distinct capability metadata sets using the binary
operation `&` (or any binary operation, for that matter).

In general, CHERI Clang takes the left-hand operand\'s capability
metadata (here, variable [a]{.title-ref}) but the compiler emits a
warning, which might be an error if you turn on the `-Werror` compile
flag.

## Avoiding this warning

The best way to avoid the warning is to consider whether the binary
operation on capability values makes sense. It would be better to
convert to a simple integer type and do the calculation in that
(non-capability) domain instead as follows

```c {3}
#include <stdint.h>

uintptr_t combine(uintptr_t a, ptraddr_t b) {
  uintptr_t c;
  c = (a&b);
  return c;
}
```

where `b` is now a `ptraddr_t` which is a simple integer. When doing
bitwise-and operation, `a`\'s metadata will be retained.

If you do continue to use capabilities, break the complex calculation
down into two-address style code with a combined assignment expression,
for example:

```c {5-6}
#include <stdint.h>

uintptr_t combine(uintptr_t a, uintptr_t b) {
  uintptr_t c;
  c = a;
  c &= b;
  return c;
}
```

which does not trigger any compiler warnings. The `c=a;` assignment
assigns `a`\'s metadata to `c`. Then the `c&=b` retains `c`\'s metadata
and throws away `b`\'s metadata.
