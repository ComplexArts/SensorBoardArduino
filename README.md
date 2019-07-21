# Complex Arts Sensor Board

This project has sample code for the [Complex Arts Sensor Board](http://complexarts.net/home/products:sensor_board).

Code is written using the [ESP32 Arduino core](https://github.com/espressif/arduino-esp32). See

https://github.com/espressif/arduino-esp32#installation-instructions

for instructions on installing the ESP32 Arduino core. 

It also depends on the 
[Sparkfun BNO08X Arduino library](https://github.com/sparkfun/SparkFun_BNO080_Arduino_Library). See

https://www.arduino.cc/en/guide/libraries  
https://learn.sparkfun.com/tutorials/installing-an-arduino-library

for more information on installing arduino libraries.

Two demos are provided:

1. HelloWorld  
   Initializes the BNO085 over SPI and prints accelerometer reading
   over serial.

2. Bluetooth  
   Initializes the ESP32 as a Bluetooth device and the BNO085 over SPI
   and prints accelerometer reading over Bluetooth.
