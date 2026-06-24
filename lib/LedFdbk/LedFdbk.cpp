// LedFdbk.cpp
#include "LedFdbk.h"

// Class Constructor
LedFdbk::LedFdbk(uint8_t lPin)
    : ledPin(lPin), feedbackOffTimer(nullptr) {}


// Initialize method
void LedFdbk::begin() {

    // Pin mode declaration
    pinMode(ledPin,OUTPUT);
    digitalWrite(ledPin,LOW);

    // Timer creation
    feedbackOffTimer = xTimerCreate(
        "feedbackOffTimer",
        pdMS_TO_TICKS(fadeTime),
        pdFALSE, // one-shot
        (void*)this, // timer identifier
        timerCallback
    );

}

// Turn speaker and led on and start timer
void LedFdbk::turnOn() {
    //Start timer
    if (feedbackOffTimer != nullptr) {
        digitalWrite(ledPin, HIGH);
        xTimerStart(feedbackOffTimer, 0);
    }
}

// Turn led off manually
void LedFdbk::turnOff() {
    // Turn off led
    digitalWrite(ledPin, LOW);
    
    // stop Timer
    if (feedbackOffTimer != nullptr) {
        xTimerStop(feedbackOffTimer, 0);
    }
}

// Turn led off after time expires
void LedFdbk::timerExpired() {
    // Turn off led
    digitalWrite(ledPin, LOW);
}

// Timer callback
void LedFdbk::timerCallback(TimerHandle_t xTimer) {
    // Get back instance using pvTimerGetTimerID
    LedFdbk* instance = static_cast<LedFdbk*>(pvTimerGetTimerID(xTimer));
    instance->timerExpired();
}