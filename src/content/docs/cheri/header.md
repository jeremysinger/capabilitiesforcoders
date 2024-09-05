---
title: Which header file(s) should I include to use CHERI functions?
description: Which headers do I need to use CHERI?
---

:::danger[TODO]
duncan says this is not universal? i.e. `cheriot` does not have `cheriintrin.h`

We should perhaps specify.
:::

To call CHERI API functions from your own C code, include the `cheriintrin.h` header file. This will be available as a system header file with CHERI C compiler toolchains. In general, CHERI API functions start with the `cheri_` prefix.

```c {1}
// example.c
#include <cheriintrin.h>
#include <stdio.h>
#include <stdlib.h>

int main() {
  void *x = (void *)malloc(1);
  printf("The bounds of this capability are %d\n",
         cheri_length_get(x));
  return 0;
}
```

## Further References:

- `cheriintrin.h` header file in the [llvm project repo](https://github.com/CTSRD-CHERI/llvm-project/blob/master/clang/lib/Headers/cheriintrin.h)

:::danger[TODO]
Jeremy please check this link is good
:::
