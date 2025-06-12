Morello CHERIBSD Server Setup
====================================

1. Download and Write Image
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

2. Install Minicom
------------------

Minicom is used to interact with devices connected via serial ports. Install it by running:

.. code-block:: bash

    sudo apt-get install minicom

3. Connect to Serial Consoles
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

4. Complete Installation
------------------------

To finish booting, follow the official guide:

    https://ctsrd-cheri.github.io/cheribsd-getting-started/morello-install/index.html
