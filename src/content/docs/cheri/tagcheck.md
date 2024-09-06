---
title: How do I know if a capability is valid?
description: How do I know if a capability is valid?
---

## What is a tag bit?

Every memory location in the system has an associated tag bit, which indicates whether or not the value in this location is a valid capability. This tag bit is often called the \'129th bit\'. It is generally stored out-of-band, in a separate metadata block.

If the tag bit is set, this means the corresponding memory location does contain a valid capability value so can be dereferenced. On the other hand, if the tag bit is not set then the value in the corresponding memory location is not a valid capability.

We can query tag bits with the `cheri_tag_get(void *capability)` function.

```c {8,11}
#include <cheriintrin.h>
#include <stdio.h>
#include <stdlib.h>

int main() {
    void *p = (void *)0xbeef;      // not valid!
    void *q = malloc(sizeof(int)); // should be ok
    if (cheri_tag_get(p)) {
      printf("p is valid\n");
    }
    if (cheri_tag_get(q)) {
      printf("q is valid\n");
    }
    return 0;
}
```
