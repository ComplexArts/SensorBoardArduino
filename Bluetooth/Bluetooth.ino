/*
 * This is a sample application that configures the BNO085 using the 
 * Sparkfun BNO080 Arduino library
 * 
 * https://github.com/sparkfun/SparkFun_BNO080_Arduino_Library
 * 
 * over SPI and prints out accerelometer readings over Bluetooth.
 * 
 * Great for checking if gravity is still around!
 * 
 * As a bonus, it also blinks the onboard user LEDs.
 */
#include <SPI.h>

#include "SparkFun_BNO080_Arduino_Library.h"
#include "BluetoothSerial.h"

// Bluetooth device
BluetoothSerial ESP_BT;
String bluetoothDeviceName = "ComplexArtsSensorBoard"; 

// IMU device
BNO080 myIMU;

// IMU pins
byte imuCSPin = 17;
byte imuWAKPin = 16;
byte imuINTPin = 35;
byte imuRSTPin = 5;

// SPI pins
byte spiCLK = 18;
byte spiMISO = 19;
byte spiMOSI = 23;

// SPI device
SPIClass spiPort;
uint32_t spiPortSpeed = 100*1000;

// Accelerometer read period (ms)
unsigned long accelPeriod = 50;

// Sleep time (should be smaller than accelPeriod) 
unsigned long sleepTime = 30;

// Setup LEDs
int incoming;
int USER_LED1 = 12;
int USER_LED2 = 13;

// LED blinking setup
int LED1 = LOW;
int LED2 = HIGH;
unsigned long prevMilli = 0;
const long blinkPeriod = 1000;

void setup()
{
    // initialize serial
    Serial.begin(115200);

    // initialize bluetooth
    // this is the name the device will identify itself
    ESP_BT.begin("ComplexArtsSensorBoard");

    // say hello
    Serial.println();
    Serial.println("   _____                               _                                     _           ");
    Serial.println("  / ____|                             | |                     /\\            | |          ");
    Serial.println(" | |        ___    _ __ ___    _ __   | |   ___  __  __      /  \\     _ __  | |_   ___   ");
    Serial.println(" | |       / _ \\  | '_ ` _ \\  | '_ \\  | |  / _ \\ \\ \\/ /     / /\\ \\   | '__| | __| / __|  ");
    Serial.println(" | |____  | (_) | | | | | | | | |_) | | | |  __/  >  <     / ____ \\  | |    | |_  \\__ \\  ");
    Serial.println("  \\_____|  \\___/  |_| |_| |_| | .__/  |_|  \\___| /_/\\_\\   /_/    \\_\\ |_|     \\__| |___/  ");
    Serial.println("                              | |                                                        ");
    Serial.println("                              |_|                                                        ");
    Serial.println("                                                                S E N S O R   B O A R D  ");
    Serial.println();
    Serial.println("Hello World Bluetooth!");
    Serial.println("Bluetooth device is ready to pair");
    Serial.println("Look for '" + bluetoothDeviceName + "'");
    
    // initialize SPI interface for the BNO085
    spiPort.begin(spiCLK, spiMISO, spiMOSI, imuCSPin);

    // uncomment to debug messages over serial
    // myIMU.enableDebugging(Serial);

    // freeze if IMU is not detected
    if (myIMU.beginSPI(imuCSPin, imuWAKPin, imuINTPin, imuRSTPin, spiPortSpeed, spiPort) == false)
    {
        Serial.println("BNO085 over SPI not detected. Freezing...");
        while(1);
    }

    // enable accelerometer, readings will be produced every 50 ms
    myIMU.enableAccelerometer(50);

    Serial.println("Accelerometer enabled");
    Serial.println("Output in form x, y, z, in m/s^2");
    Serial.println("Nothing will be printed here.");
    Serial.println("Connect to the device via Bluetooth to see the data.");

    // setup user LED pins as output
    pinMode(USER_LED1, OUTPUT);
    pinMode(USER_LED2, OUTPUT);

    delay(3000);
}

void loop()
{

    // look for reports from the IMU
    if (myIMU.dataAvailable() == true)
    {
        // got data, read it
        float x = myIMU.getAccelX();
        float y = myIMU.getAccelY();
        float z = myIMU.getAccelZ();

        // print it
        ESP_BT.print("X = ");
        ESP_BT.print(x, 4);
        ESP_BT.print(",");
        ESP_BT.print("   ");
        ESP_BT.print("Y = ");
        ESP_BT.print(y, 4);
        ESP_BT.print(",");
        ESP_BT.print("   ");
        ESP_BT.print("Z = ");
        ESP_BT.print(z, 4);
        ESP_BT.print(",");
        ESP_BT.println();

        // blink?
        unsigned long currentMilli = millis();
        if (currentMilli - prevMilli >= blinkPeriod)
        {
            // toggle LEDs
            prevMilli = currentMilli;
            LED1 = !LED1;
            LED2 = !LED2;
            digitalWrite(USER_LED1, LED1);
            digitalWrite(USER_LED2, LED2);
        }

        // got data, now go to sleep
        delay(sleepTime);
    }
}
