// SoundFdbk.cpp
#include "SoundFdbk.h"

// Class Constructor
SoundFdbk::SoundFdbk(uint8_t bzrPin)
    : buzzerPin(bzrPin), feedbackOffTimer(nullptr) {}


// Initialize method
void SoundFdbk::begin() {

    // Pin mode declaration
    pinMode(buzzerPin,OUTPUT);
    digitalWrite(buzzerPin,LOW);

    // Timer creation
    feedbackOffTimer = xTimerCreate(
        "feedbackOffTimer",
        pdMS_TO_TICKS(fadeTime),
        pdFALSE, // one-shot
        (void*)this, // timer identifier
        timerCallback
    );

}

// Turn buzzer on and start timer
void SoundFdbk::playSound(int freq) {
    //Start timer
    if (feedbackOffTimer != nullptr) {
        tone(buzzerPin,freq);
        xTimerStart(feedbackOffTimer, 0);
    }
}

// Stop buzzer sound manually
void SoundFdbk::stopSound() {
    // Stop sound
    noTone(buzzerPin);

    // Stop timer
    if (feedbackOffTimer != nullptr) {
        xTimerStop(feedbackOffTimer, 0);
    }
}

// Stop buzzer sound after time expires
void SoundFdbk::timerExpired() {
    // Stop sound
    noTone(buzzerPin);
}

// Timer callback
void SoundFdbk::timerCallback(TimerHandle_t xTimer) {
    // Get back instance using pvTimerGetTimerID
    SoundFdbk* instance = static_cast<SoundFdbk*>(pvTimerGetTimerID(xTimer));
    instance->timerExpired();
}