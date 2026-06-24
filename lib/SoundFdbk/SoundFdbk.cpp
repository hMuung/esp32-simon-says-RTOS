// SoundFdbk.cpp
#include "SoundFdbk.h"


// Constructor stores the fixed buzzer pin
SoundFdbk::SoundFdbk(uint8_t bzrPin) : buzzerPin(bzrPin) {}

// Destructor releases the timer resource
SoundFdbk::~SoundFdbk() {
    if (feedbackOffTimer != nullptr) {
        xTimerDelete(feedbackOffTimer, 0);
        feedbackOffTimer = nullptr;
    }
}

// Initializes GPIO and creates the one-shot timer
void SoundFdbk::begin() {
    pinMode(buzzerPin, OUTPUT);
    digitalWrite(buzzerPin, LOW);

    feedbackOffTimer = xTimerCreate(
        "SoundFdbkOff",
        pdMS_TO_TICKS(fadeTimeMs),
        pdFALSE,
        static_cast<void*>(this),
        timerCallback
    );
}

// Plays tone and restarts the auto-off timer
void SoundFdbk::playSound(int freq) {
    tone(buzzerPin, freq);

    if (feedbackOffTimer != nullptr) {
        xTimerStop(feedbackOffTimer, 0);
        xTimerStart(feedbackOffTimer, 0);
    }
}

// Manual stop also cancels pending auto-stop
void SoundFdbk::stopSound() {
    noTone(buzzerPin);
    digitalWrite(buzzerPin, LOW);

    if (feedbackOffTimer != nullptr) {
        xTimerStop(feedbackOffTimer, 0);
    }
}

// Timer-expired stop does not stop the already expired timer
void SoundFdbk::timerExpired() {
    noTone(buzzerPin);
    digitalWrite(buzzerPin, LOW);
}

// Static callback bridges FreeRTOS C callback to C++ instance
void SoundFdbk::timerCallback(TimerHandle_t xTimer) {
    auto* instance = static_cast<SoundFdbk*>(pvTimerGetTimerID(xTimer));

    if (instance != nullptr) {
        instance->timerExpired();
    }
}