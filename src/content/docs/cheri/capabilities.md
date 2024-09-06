---
title: What is a CHERI capability?
description: What is a CHERI Capability?
---

A capability is an unforgeable token of authority that grants particular rights to perform an action in an execution context.

In the CHERI context, a capability is a _fat pointer_ that includes not only a raw memory address, but also associated metadata for access permissions, bounds and pointer validity.
