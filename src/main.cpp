#include <Arduino.h>


// Hardware Pins
const uint8_t buttonPins[] = {1, 3, 10, 2};
const uint8_t ledPins[]    = {18, 8, 19, 9};
#define SPEAKER_PIN 0


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
