================================================
Can I run FreeBSD AArch64 binaries on CheriBSD?
================================================

AArch64 binaries compiled for FreeBSD will run on CheriBSD
as `hybrid` executables. We have tested compiling C programs
with clang on a `FreeBSD 12 <https://aws.amazon.com/marketplace/pp/prodview-wnxqeciczgenm>`_ machine image, hosted on Amazon Web Services
`t4g <https://aws.amazon.com/ec2/instance-types/t4/>`_ instances
which feature Graviton2 Arm processors. (These Graviton processors are
architecturally very similar to the
`Neoverse <https://www.arm.com/partners/aws>`_ base processor for Morello.)

The FreeBSD compiled code runs properly on CheriBSD on Morello.
