/*
  Blink

  Turns the Complex Art's Sensor Board LEDs on for one second, then off for one second, repeatedly.

  This example code is inspired by

  http://www.arduino.cc/en/Tutorial/Blink
*/

#include <ca_sensor.h>

// the setup function runs once when you press reset or power the board
void setup() {

    Serial.begin(115200);
  
    // initialize digital pin LED_BUILTIN as an output.
    pinMode(ULED1, OUTPUT);
    pinMode(ULED2, OUTPUT);
}

// the loop function runs over and over again forever
void loop() {

    Serial.print(".");
  
    digitalWrite(ULED1, HIGH);   // turn the LED 1 on  (HIGH is the voltage level)
    digitalWrite(ULED2, LOW);    // turn the LED 2 off (LOW is the voltage level)
    delay(1000);                 // wait for a second
    digitalWrite(ULED1, LOW);    // turn the LED 1 off by making the voltage LOW
    digitalWrite(ULED2, HIGH);   // turn the LED 2 on by making the voltage HIGH
    delay(1000);                 // wait for a second
}
