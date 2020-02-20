# Complex Arts Sensor Board

This project has sample code for the [Complex Arts Sensor Board](http://complexarts.net/home/products:sensor_board).

## Installation

The code is written using the [ESP32 Arduino core](https://github.com/espressif/arduino-esp32). See

https://complexarts.net/wiki/code/arduino

for instructions on installing the ESP32 Arduino core. 

In order to interact with the onboard BNO085 IMU you will need to install the  
[Sparkfun BNO08X Arduino library](https://github.com/sparkfun/SparkFun_BNO080_Arduino_Library). See

https://www.arduino.cc/en/guide/libraries  
https://learn.sparkfun.com/tutorials/installing-an-arduino-library

for more information on installing arduino libraries.

Android phones can receive serial streams over Bluetooth. See:
https://play.google.com/store/apps/details?id=de.kai_morich.serial_bluetooth_terminal

## Demos

Two demos are provided:

1. [Blink](https://github.com/ComplexArts/SensorBoardArduino/tree/master/Blink)  
   Version of the standard Blink demo that runs on the Sensor board.

2. [HelloWorld](https://github.com/ComplexArts/SensorBoardArduino/tree/master/HelloWorld)  
   Initializes the BNO085 over SPI and prints accelerometer reading
   over serial.

3. [Bluetooth](https://github.com/ComplexArts/SensorBoardArduino/tree/master/Bluetooth)  
   Initializes the ESP32 as a Bluetooth device and the BNO085 over SPI
   and prints accelerometer reading over Bluetooth.
