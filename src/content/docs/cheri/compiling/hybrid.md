---
title: How do I compile for Hybrid CHERI?
description: How do I compile for native Hybrid CHERI
---

If you have access to Morello hardware, you can compile natively on CheriBSD. You need to install llvm with pkg, then you can run it as you would on any other platform.

If you want to generate _hybrid_ code for Morello, you can use a CHERI LLVM compiler, but you need to specify the appropriate ABI flag.

```bash
pkg64 install llvm-morello
```

Then you can invoke the clang compiler to compile C programs to hybrid binaries like this:

```bash
clang-morello -march=morello -mabi=aapcs ./file.c
```

You should specify both the march and mabi flags explicitly for hybrid mode. AAPCS stands for \"Arm Architecture Procedure Call Standard\" which is the default AArch64 ABI.
