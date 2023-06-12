![sample](https://github.com/cheise/Minimal-Tachometer-Pico-RP2040/assets/59045759/34376a13-0197-4202-819e-037a49317810)


# Minimal-Tachometer-Pico-RP2040-MCP2515
Minimal Tachometer Raspberry Pico based

What do you need:

A Display, in my case a chep 12864-06D Display with SPI. Youn can use any other display supported by the U8G2 Library.
https://de.aliexpress.com/item/1005001696187818.html

A MCP2515 cheap CAN-BUS PCB (we must modified the oscillating quartz to 16 Mhz and the PCB from the MCP2515, see below, later)
https://de.aliexpress.com/item/32796488204.html

A Raspberry Pico RP2040 MCU Board or Clone
https://de.aliexpress.com/item/1005003790600416.html

A oscillating quartz 16 Mhz

A Car Power Adapter, normaly used for cellphone charging or a Stepdown Converter 12V --> 5V.

A 3D Printer (optional) or a universal case for the project.

and some libraries:

U8G2 Library: https://github.com/olikraus/u8g2

Arduino CAN:  https://github.com/sandeepmistry/arduino-CAN

Arduino OBD2: https://github.com/sandeepmistry/arduino-OBD2

all libraries are compartible with the RP2040 Core/SDK

.... more infos comming soon ....
