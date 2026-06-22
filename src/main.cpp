#include <Arduino.h>

#define LED 19

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  Serial.println("Hello, ESP32-C3!");

  pinMode(LED, OUTPUT);
}

void loop() {
  // put your main code here, to run repeatedly:
    
  digitalWrite(LED, !digitalRead(LED));
  delay(500); // this speeds up the simulation
}
