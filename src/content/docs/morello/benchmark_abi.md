---
title: What is the benchmark ABI for CHERI?
description: What is the benchmark ABI for CHERI?
---

<!-- TODO I beleive this is morello specific? from what duncan has said -->
<!-- Something to do with branch predictors and capabilities being poorly optimised for that. -->

The _benchmark ABI_ is a special mode of CHERI code execution that
overcomes some Morello performance limitations. It should only be used
for reporting experimental performance results, and such results should
be listed as _benchmark ABI results_.

## Benchmark ABI compiler flags

If you have installed LLVM natively on Morello with CheriBSD 23.11 or
later, then you will be able to compile code with the benchmark ABI
using the flag `-mabi=purecap-benchmark`. For example:

```bash
clang-morello -march=morello -mabi=purecap-benchmark ./file.c
```

If you want to check that the compiled executable file does conform to
the benchmark ABI, then use the `file` command. You should see output
similar to this:

```bash
file ./a.out
./a.out: ELF 64-bit LSB pie executable, ARM aarch64, C64, CheriABI, version 1 (SYSV), dynamically linked, interpreter /libexec/ld-elf.so.1, for FreeBSD 14.0 (1400094), FreeBSD-style, pure-capability benchmark ABI, with debug_info, not stripped
```

Executables compiled for benchmark ABI will use shared objects from
`/usr/lib64cb`. Other shared objects could be picked up from directories
specified in the environment variable named `LD_64CB_LIBRARY_PATH`.
CheriBSD ports utilities and libraries compiled for the benchmark ABI
may be installed using the `pkg64cb` package management utility.

### More information

- Tutorial page on [using the benchmark ABI](https://www.cheribsd.org/tutorial/23.11/benchmark/index.html)
- Advice about [performance reporting for Morello](https://ctsrd-cheri.github.io/morello-early-performance-results/)
