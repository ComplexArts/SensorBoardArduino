
#include <SPI.h>

#include "SparkFun_BNO080_Arduino_Library.h"
#include "BluetoothSerial.h" //Header File for Serial Bluetooth, will be added by default into Arduino

BluetoothSerial ESP_BT; //Object for Bluetooth
BNO080 myIMU;

int incoming;
int LED_BUILTIN = 12;

//These pins can be any GPIO
byte imuCSPin = 17;
byte imuWAKPin = 16;
byte imuINTPin = 35;
byte imuRSTPin = 5;

byte spiCLK = 18;
byte spiMISO = 19;
byte spiMOSI = 23;

SPIClass spiPort;
uint32_t spiPortSpeed = 100*1000;

int LEDstate1 = LOW;
int LEDstate2 = HIGH;
unsigned long prevMilli = 0;
const long interval = 1000;

void setup()
{
  Serial.begin(115200);
  Serial.println();
  Serial.println("BNO080 Read Example");
  ESP_BT.begin("ESP32_LED_Control"); //Name of your Bluetooth Signal
  Serial.println("Bluetooth Device is Ready to Pair");

  spiPort.begin(spiCLK, spiMISO, spiMOSI, imuCSPin);

  myIMU.enableDebugging(Serial); //Pipe debug messages to Serial port
  
  if(myIMU.beginSPI(imuCSPin, imuWAKPin, imuINTPin, imuRSTPin, spiPortSpeed, spiPort) == false)
  {
    Serial.println("BNO080 over SPI not detected. Are you sure you have all 6 connections? Freezing...");
    while(1);
  }

  myIMU.enableAccelerometer(50); //Send data update every 50ms

  pinMode(12, OUTPUT);
  pinMode(13, OUTPUT);

  Serial.println(F("Accelerometer enabled"));
  Serial.println(F("Output in form x, y, z, in m/s^2"));
  Serial.println("//////////////////////////////////");
  Serial.println("//////////COMPLEX ARTS////////////");
  Serial.println("//////////SENSOR_BOARD////////////");
  Serial.println("//////////////////////////////////");
  delay(3000);
}

void loop()
{
  //Look for reports from the IMU
  if (myIMU.dataAvailable() == true)
  {
    float x = myIMU.getAccelX();
    float y = myIMU.getAccelY();
    float z = myIMU.getAccelZ();

    ESP_BT.print("X = ");
    ESP_BT.print(x, 2);
    ESP_BT.print(F(","));
    ESP_BT.print("   ");
    ESP_BT.print("Y = ");
    ESP_BT.print(y, 2);
    ESP_BT.print(F(","));
    ESP_BT.print("   ");
    ESP_BT.print("Z = ");
    ESP_BT.print(z, 2);
    ESP_BT.print(F(","));

    ESP_BT.println();
    
    unsigned long currentMilli = millis();

    if(currentMilli - prevMilli >= interval){
      prevMilli = currentMilli;
      if( LEDstate1 == 0){
        LEDstate1 = 1;
      } else {
        LEDstate1 = 0;
      }
      if( LEDstate2 == 0){
        LEDstate2 = 1;
      } else {
        LEDstate2 = 0;
      }
    }
    digitalWrite(12, LEDstate1);
    digitalWrite(13, LEDstate2);
    delay(20);
  }
}
