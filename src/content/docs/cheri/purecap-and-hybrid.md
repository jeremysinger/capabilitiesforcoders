---
title: What are the Purecap and Hybrid Modes?
description: What are the Purecap and Hybrid Modes?
---

## Whats the difference between purecap and hybrid mode?

There are two modes of execution for CHERI: _purecap_ and _hybrid_. For a _purecap_ executable, all memory references are represented as capability values. On the other hand, for a _hybrid_ executable, memory references are represented as raw pointers, i.e. the code is not capability-aware.

# How do I tell whether an executable is purecap or hybrid?

CheriBSD can run both purecap and hybrid binaries. Depending on how your system is configured, the relevant libraries will be in different directories. (For instance, for purecap CheriBSD, purecap libraries are in _/usr/lib_ and hybrid libraries are in
_/usr/lib64_.)

You can use the CheriBSD `file` command to detect whether a binary file is _purecap_ (known as _C64_) or _hybrid_ (known as _A64_).

```bash
file test-purecap.exe
```

The output will look like this :

    ELF 64-bit LSB pie executable, ARM aarch64, C64,
    CheriABI, version 1 (SYSV), dynamically linked,
    interpreter /libexec/ld-elf.so.1, for FreeBSD 14.0
    (1400064), FreeBSD-style, with debug_info, not stripped

For a hybrid mode executable, the file command will show the elf file as
'A64':

```bash
file test-hybrid.exe
```

The output will look like this :

    ELF 64-bit LSB executable, ARM aarch64, A64,
    version 1 (FreeBSD), dynamically linked,
    interpreter /libexec/ld-elf.so.1, for FreeBSD 14.0
    (1400064), FreeBSD-style, with debug_info, not stripped
