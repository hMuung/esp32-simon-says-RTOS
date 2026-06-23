#include <Arduino.h>


const uint8_t buttonPins[] = {1, 3, 10, 2};


void setup() {

  Serial.begin(115200);

  for(const auto &pin : buttonPins) {
    pinMode(pin, INPUT_PULLUP);
  }
  
}

void loop() {
    
  for(const auto &button : buttonPins) {
    if(!digitalRead(button)) {
      Serial.println("button pressed");
    }
  }
  delay(10);

}
