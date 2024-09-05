---
title: How do I configure networking on CheriBSD?
description: How do I configure networking on CheriBSD?
---

Once you have installed CheriBSD, you will need to configure the
networking.

## Ethernet cable

First plug your ethernet cable into the socket labelled **GbE/USB3**. If
you need to register your Ethernet MAC address with your network
administrator, you can find this by running the interactive command:

```bash
ifconfig re0
```

via the UART / serial console. Look for the `ether` line and find the
long hex number separated by : characters. This is your MAC address.

## Configuring DHCP

If you use DHCP on your wired network (most common), then you can
configure this in the `/etc/rc.conf` file. For a machine with hostname
`morello`, your `rc.conf` might look like this:

```bash
hostname="morello"
keymap="uk.kbd"
ifconfig_re0="DHCP"
sshd_enable="YES"
ntpd_enable="YES"
```

Now try to `ping google.com` and see if it works\...

## Configuring sshd

So far, you have probably been connected over the serial/USB terminal.
Or maybe you are directly running on the Morello board with a USB
keyboard and HDMI monitor. The next step is to enable remote access via
ssh. You might want to edit the file `/etc/ssh/sshd_config`. The
modifications below will enable remote login for root and password login
for all users. These are reasonable options if you are on a secure (i.e.
not public) network.

```bash
# Authentication:
PermitRootLogin yes
PasswordAuthentication yes
```

Hopefully now you can access your Morello CheriBSD server via `ssh` from
another machine on the network.

## Further References:

- Official FreeBSD docs on [setting up networking](https://docs.freebsd.org/en/books/handbook/network/)
  (Since CheriBSD is based on FreeBSD these instructions are directly
  applicable)
