WUSATP1
=======

Android controller code for Prototype 1

The first prototype of the CubeSat for 2012/3 was designed to be controlled by two independant Arduino Leonardos performing similar but different functions.

Arduino 1 - Comms Build
- XBee-PRO 868
- Linksprite JPEG camera
- RTC DS1307+
- MicroSD (SPI mode)
- 2x Thermocouple
- Pressure sensor

Arduino 2 - GPS Build
- GPS
- Linksprite JPEG camera
- RTC DS1307+
- MicroSD (SPI mode)
- 2x Thermocouple
- Pressure sensor

Building the code required a modification to the SoftwareSerial libray.  See the comments in the Camera.ino file.
