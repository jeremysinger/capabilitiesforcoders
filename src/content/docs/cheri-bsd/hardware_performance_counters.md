---
title: How do I use hardware performance counters on Morello with CheriBSD?
description: How do I use hardware performance counters on Morello with CheriBSD?
---

Hardware performance counters are supported by the Arm Performance
Monitoring Unit (PMU) within the Morello processor. Specific performance
monitoring events can track micro-architectural behaviour.

First we need to load the CheriBSD kernel module for hardware
performance monitoring:

```bash
kldload hwpmc.ko
```

Now we can list the set of performance monitoring events that are
available:

```bash
pmccontrol -L
```

which should return a long list of events, a little like this:

    SOFT
     CLOCK.HARD
     CLOCK.STAT
     INTR.ALL
     INTR.ITHREAD
     INTR.FILTER
     INTR.STRAY
     INTR.SCHEDULE
     LOCK.FAILED
      INTR.WAITING
     CLOCK.PROF
    ARMV8
     SW_INCR
     L1I_CACHE_REFILL
     L1I_TLB_REFILL
     L1D_CACHE_REFILL
     L1D_CACHE
     L1D_TLB_REFILL
     INST_RETIRED
     EXC_TAKEN
     EXC_RETURN
     CID_WRITE_RETIRED
     BR_MIS_PRED
     CPU_CYCLES
     BR_PRED
     MEM_ACCESS
     L1I_CACHE
     ...

Note that Morello has additional capability-specific events, including:

    MEM_ACCESS_RD_CTAG
    MEM_ACCESS_WR_CTAG
    CAP_MEM_ACCESS_RD
    CAP_MEM_ACCESS_WR

To report an event count for a single process and its children, e.g.
`L1D_CACHE`, you use the `pmcstat` command:

```bash
pmcstat -d -w 100 -p L1D_CACHE  ls
```

This runs the `ls` command, counts the number of L1 data cache accesses
for the process execution, and reports the result every 100 seconds (or
before if the process exits sooner, which it should do.)

The output looks like this:

    barnes.elf       espresso.elf
    cfrac.elf        glibc_bench_simple.elf
    # p/L1D_CACHE
         1957206

for a directory containing four .elf files. This command execution took
almost 2 million L1 data accesses.

The `pmcstat` tool is extremely powerful and can generate annotated
stack traces attributing event counts to program functions. This is
helpful for runtime profiling. Below is an example that profiles
LL_CACHE_MISS_RD events on a binary_tree.elf executable, generating an
intermediate file `out.pmc` that is post-processed to give a gprof style
profile dump `out.stacks`..

```bash
pmcstat -S LL_CACHE_MISS_RD -O out.pmc ./binary_tree.elf
pmcstat -R out.pmc -z 32 -G out.stacks
cat out.stacks
```

The output looks like this:

:

    @ MEM_ACCESS [138 samples]

    60.14%  [83]       memmove_c @ /boot/kernel/kernel
     100.0%  [83]        uiomove_fromphys_flags
      100.0%  [83]         ffs_write
       100.0%  [83]          VOP_WRITE_APV
        100.0%  [83]           vn_write
     100.0%  [83]            vn_io_fault
      100.0%  [83]             fork_exit

    02.90%  [4]        __mtx_unlock_flags @ /boot/kernel/kernel
     25.00%  [1]         g_io_deliver
      100.0%  [1]          g_disk_done
       100.0%  [1]           xpt_done_process
        100.0%  [1]            xpt_done_direct
     100.0%  [1]             ahci_ch_intr_direct
      100.0%  [1]              ahci_intr
       100.0%  [1]               ithread_loop
        100.0%  [1]                fork_exit
    (snipped)

## Further References:

- Arm [Architecture Reference
  Manual](https://developer.arm.com/documentation/ddi0487/latest)
  (look for performance monitors, but watch out because the manual is
  a 75MB PDF download!)
- [Morello
  supplement](https://developer.arm.com/documentation/ddi0606/latest)
  to Arm Architecture Reference Manual (look for performance monitors)
- Brendan Gregg\'s [slides on performance
  analysis](https://www.brendangregg.com/Slides/MeetBSD2014_Performance/#1)
  in FreeBSD, for lots of useful info about `pmcstat`
