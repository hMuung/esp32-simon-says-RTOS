// LedSound.cpp
#include "LedSound.h"

// Static shared pin
uint8_t LedSound::sharedSpeakerPin = 0;

// Class Constructor
LedSound::LedSound(uint8_t lPin, int freq)
    : ledPin(lPin), toneFreq(freq), feedbackOffTimer(nullptr) {}


// Initialize method
void LedSound::begin() {

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
void LedSound::turnOn() {
    digitalWrite(ledPin, HIGH);
    tone(LedSound::sharedSpeakerPin, toneFreq);
    
    //Start timer
    xTimerStart(feedbackOffTimer, 0);

    // In case of failure
    if (feedbackOffTimer != nullptr) {
        xTimerStart(feedbackOffTimer, 0);
    }
}


// Turn speaker and led off
void LedSound::turnOff() {
    digitalWrite(ledPin, LOW);
    noTone(LedSound::sharedSpeakerPin);
}

// Timer callback
void LedSound::timerCallback(TimerHandle_t xTimer) {
    // Get back instance using pvTimerGetTimerID
    LedSound* instance = static_cast<LedSound*>(pvTimerGetTimerID(xTimer));
    instance->turnOff();
}