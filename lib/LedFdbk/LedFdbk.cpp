// LedFdbk.cpp
#include "LedFdbk.h"


// Constructor stores the fixed LED pin
LedFdbk::LedFdbk(uint8_t lPin) : ledPin(lPin) {}

// Destructor releases the timer resource
LedFdbk::~LedFdbk() {
    if (feedbackOffTimer != nullptr) {
        xTimerDelete(feedbackOffTimer, 0);
        feedbackOffTimer = nullptr;
    }
}

// Initializes GPIO and creates the one-shot timer
void LedFdbk::begin() {
    pinMode(ledPin, OUTPUT);
    setLed(false);

    feedbackOffTimer = xTimerCreate(
        "LedFdbkOff",
        pdMS_TO_TICKS(fadeTimeMs),
        pdFALSE,
        static_cast<void*>(this),
        timerCallback
    );
}

// Centralizes LED writes
void LedFdbk::setLed(bool state) {
    digitalWrite(ledPin, state ? HIGH : LOW);
}

// Turns LED on and restarts the auto-off timer
void LedFdbk::turnOn() {
    setLed(true);

    if (feedbackOffTimer != nullptr) {
        xTimerStop(feedbackOffTimer, 0);
        xTimerStart(feedbackOffTimer, 0);
    }
}

// Manual off also cancels pending auto-off
void LedFdbk::turnOff() {
    setLed(false);

    if (feedbackOffTimer != nullptr) {
        xTimerStop(feedbackOffTimer, 0);
    }
}

// Timer-expired off does not stop the already expired timer
void LedFdbk::timerExpired() {
    setLed(false);
}

// Static callback bridges FreeRTOS C callback to C++ instance
void LedFdbk::timerCallback(TimerHandle_t xTimer) {
    auto* instance = static_cast<LedFdbk*>(pvTimerGetTimerID(xTimer));

    if (instance != nullptr) {
        instance->timerExpired();
    }
}