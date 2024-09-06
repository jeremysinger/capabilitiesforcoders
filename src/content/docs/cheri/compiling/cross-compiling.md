---
title: Cross Compiling for CHERI
description: Cross compiling for CHERI
---

## Cheribuild LLVM

Cheribuild's LLVM compiler runs in cross-compilation mode for CHERI, so you can compile to CHERI targets from a non-CHERI platform like `x86_64/linux`. By default your CHERI LLVM toolchain will be located in the _cheri_ directory in your home directory.

There is a configuration file with the relevant parameters set up to enable cross-compilation. One convenient way to invoke the cross-compiler is by defining appropriate environment variables:

```bash
export CC=~/cheri/output/morello-sdk/bin/clang
export CFLAGS="--config cheribsd-morello-hybrid.cfg"
```

You can then invoke the cross-compiler on your non-CHERI platform:

```bash
$CC $CFLAGS test.c
```

which will generate an `a.out` executable file that you can copy to a CHERI system to execute.

Cross-compilation is most useful when you are running a CHERI emulator and the native CHERI compiler runs too slowly on the emulator.

## Using a standard AArch64 compiler

If you compile C code natively on (say) FreeBSD and generate AArch64 code, this should work on CheriBSD as a hybrid binary. Of course, it's worth checking compatibility between compiler ABIs, FreeBSD/CheriBSD versions and shared objects, but this might be a relatively straightforward way to generate hybrid executables for CheriBSD.
