===========================================================
How do I remove the motherboard from the Morello ATX case?
===========================================================


Open the case
-------------

Unscrew (by hand) the two large black case screws connecting the glass plate to the case. You can then slide the glass plate off the side of the case to
allow physical access to the motherboard.

.. image:: ../gfx/1_case.jpg
  :width: 600
  :alt: Case with screw locations labelled

.. image:: ../gfx/15_screws.jpg
  :width: 200
  :alt: Large black screws removed from case




Disconnect the cables
---------------------

Disconnect all the cables that are plugged into the motherboard from
external components.
These are:


1. three small white fan power connectors, one of which is concealed under an ATX power cable
2. two black ribbon cables for ATX power supply
3. the red SATA cable for the hard disc
4. the four small black cables that connect motherboard pins to the front of case electronics (for reset button, power LED, HDD LED, power button). These connectors have labels on them, to avoid mixing them up - see image below. Note that each cable connects two pins, effectively it is a pair of single wire cables.
   
You can leave the two blue ribbon cables connecting to SOC-JTAG and C2C external interfaces on the motherboard, since these connect directly to the backplane which is only attached to the motherboard, not to the case.


.. image:: ../gfx/2_wires.jpg
  :width: 600
  :alt: Wires to be disconnected from motherboard

.. image:: ../gfx/25_connectors.jpg
  :width: 200
  :alt: Close-up of edge connectors from motherboard to front panel



Unscrew the motherboard
-----------------------

There are eight black metal Philips screws and one white plastic screw, which requies a flathead screwdriver. Remove all screws and the motherboard should lift out of the case.

.. image:: ../gfx/3_screws.jpg
  :width: 600
  :alt: Screws to be removed, with positions marked


Remove the RAM (optional)
-------------------------

For ease of transport, I unplugged the two DDR4 RAM modules from the motherboard, simply unclipping them from their plastic cases.

.. image:: ../gfx/5_drams.jpg
  :width: 200
  :alt: DRAMs removed from sockets

Extract motherboard from case
-----------------------------

Now the motherboard should simply lift out of the ATX case. Be careful to avoid damaging any components. Once I removed my motherboard, I immediately put it into a large anti-static bag for safety.

.. image:: ../gfx/4_board.jpg
  :width: 600
  :alt: DRAMs removed from sockets

	

Now the motherboard can be re-housed in an alternative case, perhaps a smaller ATX case with a nano PSU. Alternatively, follow the steps in reverse to re-install the motherboard in its original case.
