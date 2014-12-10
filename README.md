OBD2-wifi-forwarder
===================

Forwards [OBD2](http://en.wikipedia.org/wiki/On-board_diagnostics) raw data through wireless network as a COM port on [Raspberry Pi](http://en.wikipedia.org/wiki/Raspberry_Pi).

###Contents:
1. [Requirements](#Requirements)
2. [Setup](#Setup)

Requirements
============
 - Any model of Raspberry Pi;
 - A WiFi dongle capable of being an [Access Point](http://en.wikipedia.org/wiki/Wireless_access_point). For officially tested and supported list [see this](http://elinux.org/RPi_VerifiedPeripherals#USB_Wi-Fi_Adapters);
 - SD card equiped with Linux;
 - [PiCAN CAN-Bus board](http://skpang.co.uk/catalog/pican-canbus-board-for-raspberry-pi-p-1196.html)
Setup
=====

*Initially this project was created on Raspberry Pi with [MINIBIAN operating system](http://minibianpi.wordpress.com/). Since MINIBIAN is a minimal cut out version of [Raspbian](http://www.raspbian.org/), there might be several packages that will be needed throughout setuping up.*

 - Setup your Raspberry Pi as an access point. For starters, this guide fits well [Setting up a Raspberry Pi as a WiFi access point](https://learn.adafruit.com/setting-up-a-raspberry-pi-as-a-wifi-access-point);
 - In projects folder execute `make` to get compiled executables.
 - Run it