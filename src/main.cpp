#include <Arduino.h>
#include <freertos/FreeRTOS.h>
#include <freertos/task.h>

// Hardware pin mappings
const uint8_t buttonPins[] = {1, 3, 10, 2};

// Volatile state variables for ISR-to-main communication
volatile int lastButton = -1;
volatile uint32_t lastInterruptTime[] = {0, 0, 0, 0};

// Software debounce interval in milliseconds
const int bounceThreshold = 200;

// Function prototypes
void IRAM_ATTR handleButtonInterrupt(uint8_t);

// Specific ISR wrappers to route hardware interrupts to the common handler
void IRAM_ATTR button0ISR() { handleButtonInterrupt(0); }
void IRAM_ATTR button1ISR() { handleButtonInterrupt(1); }
void IRAM_ATTR button2ISR() { handleButtonInterrupt(2); }
void IRAM_ATTR button3ISR() { handleButtonInterrupt(3); }


void setup() {
  Serial.begin(115200);

  // Configure pins with internal pull-ups
  for(int i{}; i < 4; i++) {
    pinMode(buttonPins[i], INPUT_PULLUP);
  }

  // Attach falling-edge hardware interrupts to each wrapper
  attachInterrupt(digitalPinToInterrupt(buttonPins[0]), button0ISR, FALLING);
  attachInterrupt(digitalPinToInterrupt(buttonPins[1]), button1ISR, FALLING);
  attachInterrupt(digitalPinToInterrupt(buttonPins[2]), button2ISR, FALLING);
  attachInterrupt(digitalPinToInterrupt(buttonPins[3]), button3ISR, FALLING); 
}

void loop() {
  // Check for button presses from the ISR
  if (lastButton > -1) {
    Serial.println(lastButton);
    lastButton = -1; // Reset flag after processing
  }
}

// Core ISR debounces the input and registers the button press
void IRAM_ATTR handleButtonInterrupt(uint8_t buttonIndex) {
  uint32_t currentTime = millis();

  // Ignore interrupts triggered within the debounce threshold
  if ((currentTime - lastInterruptTime[buttonIndex]) >= bounceThreshold) {
    lastInterruptTime[buttonIndex] = currentTime;
    lastButton = buttonIndex;
  }
}