/*
 * This is a sample application that configures the BNO085 using the 
 * Sparkfun BNO080 Arduino library
 * 
 * https://github.com/sparkfun/SparkFun_BNO080_Arduino_Library
 * 
 * over SPI and prints out accerelometer readings over serial.
 * 
 * Great for checking if gravity is still around!
 * 
 * As a bonus, it also blinks the onboard user LEDs.
 */

#include <MicroNMEA.h>

// GPS device
byte GPSRX = 27;
byte GPSTX = 26;
char nmeaBuffer[100];
MicroNMEA nmea(nmeaBuffer, sizeof(nmeaBuffer));

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
    // initialize serial2
    Serial2.begin(9600, SERIAL_8N1, GPSRX, GPSTX);
  
    // initialize serial
    Serial.begin(115200);

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
    Serial.println("Hello World!");

    // setup user LED pins as output
    pinMode(USER_LED1, OUTPUT);
    pinMode(USER_LED2, OUTPUT);

    //delay(3000);
}

void loop()
{
    // look for reports from the IMU
    while (Serial2.available()) {
      char c = Serial2.read();
      Serial.print(c);
      if (nmea.process(c)) {

          // Output GPS information from previous second
          Serial.print("Valid fix: ");
          Serial.println(nmea.isValid() ? "yes" : "no");
      
          Serial.print("Nav. system: ");
          if (nmea.getNavSystem())
            Serial.println(nmea.getNavSystem());
          else
            Serial.println("none");
      
          Serial.print("Num. satellites: ");
          Serial.println(nmea.getNumSatellites());
      
          Serial.print("HDOP: ");
          Serial.println(nmea.getHDOP()/10., 1);
          
          Serial.print("Date/time: ");
          Serial.print(nmea.getYear());
          Serial.print('-');
          Serial.print(int(nmea.getMonth()));
          Serial.print('-');
          Serial.print(int(nmea.getDay()));
          Serial.print('T');
          Serial.print(int(nmea.getHour()));
          Serial.print(':');
          Serial.print(int(nmea.getMinute()));
          Serial.print(':');
          Serial.println(int(nmea.getSecond()));
      
          long latitude_mdeg = nmea.getLatitude();
          long longitude_mdeg = nmea.getLongitude();
          Serial.print("Latitude (deg): ");
          Serial.println(latitude_mdeg / 1000000., 6);
      
          Serial.print("Longitude (deg): ");
          Serial.println(longitude_mdeg / 1000000., 6);
      
          long alt;
          Serial.print("Altitude (m): ");
          if (nmea.getAltitude(alt))
            Serial.println(alt / 1000., 3);
          else
            Serial.println("not available");
      
          Serial.print("Speed: ");
          Serial.println(nmea.getSpeed() / 1000., 3);
          Serial.print("Course: ");
          Serial.println(nmea.getCourse() / 1000., 3);
        
      }
 
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
        //delay(sleepTime);
    }
}
