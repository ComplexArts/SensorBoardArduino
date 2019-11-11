#include <SPI.h>
#include <WiFi.h>
#include <WiFiUdp.h>
#include "SparkFun_BNO080_Arduino_Library.h"

/* WiFi network name and password */
const char * ssid = "Cgreen";
const char * pwd = "atm05phere";

const char * udpAddress = "255.255.255.255";
const int udpPort = 7401;

WiFiUDP udp;
BNO080 bno;

//SPI pins to the BNO
byte imuCSPin = 17;
byte imuWAKPin = 16;
byte imuINTPin = 35;
byte imuRSTPin = 5;

byte spiCLK = 18;
byte spiMISO = 19;
byte spiMOSI = 23;

SPIClass spiPort;
uint32_t spiPortSpeed = 100*1000;

void setup()
{
  //start serial
  Serial.begin(115200);
  
  //Set up WiFi communication:
  WiFi.begin(ssid, pwd);
  Serial.println();
  //wait for it...  
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  //enable UDP
  udp.begin(udpPort);
  Serial.println("");

  //print network and IP
  Serial.print("Connected to ");
  Serial.println(ssid);
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());
  
  Serial.println(" BNO085 / Rotation Vector / WiFi ");
  //start SPI port
  spiPort.begin(spiCLK, spiMISO, spiMOSI, imuCSPin);
  bno.enableDebugging(Serial); //Pipe debug messages to Serial port

  //check IMU on SPI port
  if(bno.beginSPI(imuCSPin, imuWAKPin, imuINTPin, imuRSTPin, spiPortSpeed, spiPort) == false)
  {
    Serial.println("BNO080 over SPI not detected. Are you sure you have all 6 connections? Freezing...");
    while(1);
  }

  //turn rotation vectoring on 
  bno.enableRotationVector(50); //Send data update every 50ms

}

//the float mapping function
float mapFloat(float x, float in_min, float in_max, float out_min, float out_max)
{
 return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}

float oldX, oldY, oldZ, difX, difY, difZ, Sx, Sy, Sz = 0;

void loop()
{
  //Look for reports from the IMU
  if (bno.dataAvailable() == true)
  {
    //get all the quaternion junk
    float x = bno.getQuatI();
    float y = bno.getQuatJ();
    float z = bno.getQuatK();
    
    //Print all the quaternion junk 
    Serial.print(x, 2);
    Serial.print(F(","));
    Serial.print("\t");
    Serial.print(y, 2);
    Serial.print(F(","));
    Serial.print("\t");
    Serial.print(z, 2);
    Serial.print(F(","));
    Serial.println();

    //map all the quaternion junk to polar  
    float Nx, Ny, Nz;     
   
    Nx = mapFloat(x, -1, 1, 0, 255);
    Ny = mapFloat(y, -1, 1, 0, 255); 
    Nz = mapFloat(z, -1, 1, 0, 255);     
     
    //put it in the buffer and send it along 
    uint8_t buffer[50] = {Sx,Sy,Sz};          
    udp.beginPacket(udpAddress, udpPort);    
    udp.write(buffer, 3);                     
    udp.endPacket();                         
    memset(buffer, 0, 50);

    //print the mapped coordinates
    Serial.print(Nx, 2);          
    Serial.print(F(","));        
    Serial.print("\t");           
    Serial.print(Ny, 2);          
    Serial.print(F(","));         
    Serial.print("\t");           
    Serial.print(Nz, 2);          
    Serial.println(F(","));
  }

}
