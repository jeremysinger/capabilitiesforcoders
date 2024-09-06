---
title: What is CHERI?
description: What is CHERI?
---

:::danger[TODO]
this
:::

CHERI stands for 'Capability Hardware Enhanced RISC Instructions': this is a bolt-on to a standard RISC instruction set architecture to provide support for capabilities in the processor architecture. The prototype [Arm Morello](https://www.arm.com/architecture/cpu/morello) processor supports capabilities natively in hardware.

---

## No Literal Pointers

CHERI capabilities cannot just be summoned out of nowhere; they must have providence. Practically speaking, this means that capabilities can be derived (or made from) other capabilities, but not simply created from nothing. You cannot simply write for example

```c
void *ptr = 0x0f00;
```

In a typical C program, this will probably segfault. but on the off chance that `0x0f00` is within the address space of our program, it would be OK.
Not so in CHERI. This `ptr` lacks providence, and so it is not valid.

## Monotonicity

CHERI capabilities are also _monotonic_. This means that a new capability cannot be more powerful than the capability it was derived from.

---

This page should include;

- disambiguation of different implementations
  - e.g. cheriot, cheribsd, morello
- cheribuild
-
