#include <Arduino.h>
#include <freertos/FreeRTOS.h>
#include <freertos/task.h>

#include <pitches.h>

// Hardware pin mappings
const uint8_t buttonPins[] = {1, 3, 10, 2};
const uint8_t ledPins[] = {18, 8, 19, 9};
#define SPEAKER_PIN 0

// Volatile state variables for ISR-to-main communication
volatile int lastButton = -1;
volatile uint32_t lastInterruptTime[] = {0, 0, 0, 0};

// FreeRTOS timer array for leds
TimerHandle_t ledTimers[4];

// Tone button array
const int gameTones[] = { NOTE_G3, NOTE_C4, NOTE_E4, NOTE_G4};

// Software intervals in milliseconds
const int bounceThreshold = 150;
const int ledFadeTime = 150;

// Function prototypes
void IRAM_ATTR handleButtonInterrupt(uint8_t);
void ledOffCallback(TimerHandle_t xTimer);

// Specific ISR wrappers to route hardware interrupts to the common handler
void IRAM_ATTR button0ISR() { handleButtonInterrupt(0); }
void IRAM_ATTR button1ISR() { handleButtonInterrupt(1); }
void IRAM_ATTR button2ISR() { handleButtonInterrupt(2); }
void IRAM_ATTR button3ISR() { handleButtonInterrupt(3); }


void setup() {

  // Configure pins with internal pull-ups
  for (int i{}; i < 4; i++) {
    pinMode(buttonPins[i], INPUT_PULLUP);
    pinMode(ledPins[i],OUTPUT);
    digitalWrite(ledPins[i],LOW);
  }

  // Timers creation
  for (int i{}; i < 4; i++) {
    ledTimers[i] = xTimerCreate(
      "ledTimer",
      ledFadeTime,
      pdFALSE,
      (void*)i,
      ledOffCallback
    );
  }

  // Initialize the tone generator
  tone(SPEAKER_PIN, 440, 1); 

  // Attach falling-edge hardware interrupts to each wrapper
  attachInterrupt(digitalPinToInterrupt(buttonPins[0]), button0ISR, FALLING);
  attachInterrupt(digitalPinToInterrupt(buttonPins[1]), button1ISR, FALLING);
  attachInterrupt(digitalPinToInterrupt(buttonPins[2]), button2ISR, FALLING);
  attachInterrupt(digitalPinToInterrupt(buttonPins[3]), button3ISR, FALLING); 
}

void loop() {}

// Core ISR debounces the input and registers the button press
void IRAM_ATTR handleButtonInterrupt(uint8_t buttonIndex) {
  uint32_t currentTime = millis();

  // Ignore interrupts triggered within the debounce threshold
  if ((currentTime - lastInterruptTime[buttonIndex]) >= bounceThreshold) {

    lastInterruptTime[buttonIndex] = currentTime;
    lastButton = buttonIndex;
    digitalWrite(ledPins[buttonIndex],HIGH);
    tone(SPEAKER_PIN,gameTones[buttonIndex]);

    BaseType_t xHigherPriorityTaskWoken = pdFALSE;
    xTimerStartFromISR(ledTimers[buttonIndex], &xHigherPriorityTaskWoken);
    if (xHigherPriorityTaskWoken) {
        portYIELD_FROM_ISR();
    }
    
  }
}

void ledOffCallback(TimerHandle_t xTimer) {
  uint32_t button = (uint32_t) pvTimerGetTimerID(xTimer);
  digitalWrite(ledPins[button],LOW);
  noTone(SPEAKER_PIN);
}