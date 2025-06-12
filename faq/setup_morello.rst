====================================
How do I setup a Morello CHERIBSD Server
====================================

How to download and write a CHERIBSD Image to USB
---------------------------

First, download Morello CHERI. The current version is available at:

    https://www.cheribsd.org/

The version this guide uses is **cheribsd-memstick-arm64-aarch64c-25.03.img.xz**.

Decompress the installer:

.. code-block:: bash

    nxz cheribsd-memstick-arm64-aarch64c-25.03.img.xz

Then write the disk image to a USB:

.. code-block:: bash

    dd if=cheribsd-memstick-arm64-aarch64c-25.03.img of=/dev/<USB> bs=1M

How to install Minicom
------------------

Minicom is used to interact with devices connected via serial ports. Install it by running:

.. code-block:: bash

    sudo apt-get install minicom

How to Connect to Serial Consoles
-----------------------------

"First, connect to the MCC firmware console. The serial device for the firmware on the Morello server is `ttyUSB0`.

The `-b 115200` option sets the baud rate, which matches the Morello server's configuration."

.. code-block:: bash

    sudo minicom  -D /dev/ttyUSB0 -b 115200

To access the main platform boot (run this in a separate terminal), connect using `ttyUSB2`:

.. code-block:: bash

    sudo minicom  -D /dev/ttyUSB2 -b 115200

It may be need to reboot the MCC firmware if the main platform console does not reach the steps the guide outlines in Stage 4.
If this happens run the following command in the MCC firmware console:

.. code-block:: bash

    REBOOT

Complete Installation
------------------------

To finish booting, follow the official guide:

    https://ctsrd-cheri.github.io/cheribsd-getting-started/morello-install/index.html
