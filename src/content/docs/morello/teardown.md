---
title: How do I remove the motherboard from the Morello ATX case?
description: How do I remove the motherboard from the Morello ATX case?
---

## Open the case

Unscrew (by hand) the two large black case screws connecting the glass
plate to the case. You can then slide the glass plate off the side of
the case to allow physical access to the motherboard.

![Case with screw locations labelled](/gfx/1_case.jpg)

![Large black screws removed from case](/gfx/15_screws.jpg)

## Disconnect the cables

Disconnect all the cables that are plugged into the motherboard from
external components. These are:

1.  three small white fan power connectors, one of which is concealed
    under an ATX power cable
2.  two black ribbon cables for ATX power supply
3.  the red SATA cable for the hard disc
4.  the four small black cables that connect motherboard pins to the
    front of case electronics (for reset button, power LED, HDD LED,
    power button). These connectors have labels on them, to avoid mixing
    them up - see image below. Note that each cable connects two pins,
    effectively it is a pair of single wire cables.

You can leave the two blue ribbon cables connecting to SOC-JTAG and C2C
external interfaces on the motherboard, since these connect directly to
the backplane which is only attached to the motherboard, not to the
case.

![Wires to be disconnected from motherboard](/gfx/2_wires.jpg)

![Close-up of edge connectors from motherboard to front panel](/gfx/25_connectors.jpg)

## Unscrew the motherboard

There are eight black metal Philips screws and one white plastic screw,
which requies a flathead screwdriver. Remove all screws and the
motherboard should lift out of the case.

![Screws to be removed, with positions marked](/gfx/3_screws.jpg)

## Remove the RAM (optional)

For ease of transport, I unplugged the two DDR4 RAM modules from the
motherboard, simply unclipping them from their plastic cases.

![DRAMs removed from sockets](/gfx/5_drams.jpg)

## Extract motherboard from case

Now the motherboard should simply lift out of the ATX case. Be careful
to avoid damaging any components. Once I removed my motherboard, I
immediately put it into a large anti-static bag for safety.

![DRAMs removed from sockets](/gfx/4_board.jpg)

Now the motherboard can be re-housed in an alternative case, perhaps a
smaller ATX case with a nano PSU. Alternatively, follow the steps in
reverse to re-install the motherboard in its original case.
