---
title: How do I install gdb on Morello?
description: How do I install gdb on Morello?
---

<!-- This seems to be cheriBSD specific -->

The GNU debugger gdb has been extended with support for CHERI
capabilities. There is a [github repo](https://github.com/CTSRD-CHERI/gdb).

## For native CheriBSD

If you are using `pkg64` there is a special CHERI-aware
version of gdb-cheri. This runs as a hybrid-mode executable on Morello
CheriBSD.

```bash
pkg64 install gdb-cheri
```

## For QEMU with cheribuild.py

If you are running emulated CHERI on qemu, you can use the cheribuild
target run-morello-purecap and gdb will be built as part of this target.
It\'s in the directory [/usr/local/bin/gdb]{.title-ref}.
