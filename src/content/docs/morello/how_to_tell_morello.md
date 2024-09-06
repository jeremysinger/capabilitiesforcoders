---
title: How do I tell whether I am on a Morello machine?
description: How do I tell whether I am on a Morello machine?
---

Every Arm processor has a _main id register_ `MDIR_EL1` which contains a unique CPUID identity number for the system. The full 32-bit CPUID value for Morello is `0x3F0F4120`, as specified in the [Morello Technical Reference Manual](https://developer.arm.com/documentation/102278/latest).

You can query the main id register using the C code below, with the inline assembler to fetch the `MDIR_EL1` value.

```c {12}
//check-morello.c
#include <stdio.h>

/* see Arm Morello System Development Platform
 * Technical Reference Manual
 * Document ID: 102278_0001_04_en Issue: 04 Rainier clusters
 * Appendix B2 Main ID Register, MIDR_EL1
 */
#define MORELLO 0x3f0f4120

int main() {
  long arm_cpuid;
  __asm__("mrs %0, MIDR_EL1" : "=r"(arm_cpuid));
  printf("CPU ID %lx\n", arm_cpuid);
  if (arm_cpuid == MORELLO)
    printf("This is a Morello prototype system\n");
  return 0;
}
```
