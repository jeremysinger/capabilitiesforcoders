---
title: How do I specify where shared object files are located?
description: How do I specify where shared object files are located?
---

The `LD_LIBRARY_PATH` environment variable is used in CheriBSD, for
purecap `.so` files that need to be dynamically linked with a purecap
executable.

On the other hand, the `LD_64_LIBRARY_PATH` environment variable is used
in CheriBSD for hybrid `.so` files that need to be dynamically linked
with a hybrid executable.

In Bash,

```bash
export LD_LIBRARY_PATH=/path/to/shared/object/files
# or for hybrid code ...
export LD_64_LIBRARY_PATH=/path/to/shared/object/files
```

or for C shell:

```csh
setenv LD_LIBRARY_PATH /path/to/shared/object/files
# or for hybrid code ...
setenv LD_64_LIBRARY_PATH /path/to/shared/object/files
```
