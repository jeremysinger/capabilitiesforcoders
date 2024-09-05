---
title: Can I run FreeBSD AArch64 binaries on CheriBSD?
description: Can I run FreeBSD AArch64 binaries on CheriBSD?
---

Yes, AArch64 binaries compiled for FreeBSD will run on CheriBSD as _hybrid_ executables. We have tested compiling C programs with clang on a [FreeBSD 12](https://aws.amazon.com/marketplace/pp/prodview-wnxqeciczgenm) machine image, hosted on Amazon Web Services [t4g](https://aws.amazon.com/ec2/instance-types/t4/) instances which feature Graviton2 Arm processors. (These Graviton processors are architecturally very similar to the [Neoverse](https://www.arm.com/partners/aws) base processor for
Morello.)

The FreeBSD compiled code runs properly on CheriBSD on Morello.
