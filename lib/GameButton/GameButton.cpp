#pragma once
#include <Arduino.h>
#include <freertos/FreeRTOS.h>
#include <freertos/task.h>

#include <GameButton.h>

// Static shared pin
uint8_t GameButton::sharedSpeakerPin = 0;

// Class Constructor
GameButton::GameButton(uint8_t btnPin, uint8_t lPin, int freq)
    : buttonPin(btnPin), ledPin(lPin), toneFreq(freq), 
    lastInterruptTime(0), feedbackOffTimer(nullptr) {}


// Initialize method
void GameButton::begin() {

    // Pin mode declaration
    pinMode(buttonPin,INPUT_PULLUP);
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

    // Attacht interrupt
    attachInterruptArg(
        digitalPinToInterrupt(buttonPin),
        isrWrapper,
        this,// argument to the isrWrapper
        FALLING
    );

}

// Turn speaker and led off
void GameButton::turnOff() {
    digitalWrite(ledPin, LOW);
    noTone(GameButton::sharedSpeakerPin);
}

// Interrupt handeling
void IRAM_ATTR GameButton::handleInterrupt() {
    
    uint32_t currentTime = millis(); 

        // Bounce software handeling
        if ((currentTime - lastInterruptTime) >= bounceThreshold) {
            lastInterruptTime = currentTime;
            
            digitalWrite(ledPin, HIGH);
            tone(GameButton::sharedSpeakerPin, toneFreq);

            BaseType_t xHigherPriorityTaskWoken = pdFALSE;
            xTimerStartFromISR(feedbackOffTimer, &xHigherPriorityTaskWoken);
            if (xHigherPriorityTaskWoken) {
                portYIELD_FROM_ISR();
            }
        }
}


// ISR Wrapper
void IRAM_ATTR GameButton::isrWrapper(void* arg) {
    // Cast argument back to GameButton instance
    GameButton* instance = static_cast<GameButton*>(arg);
    instance->handleInterrupt();
}

// Timer callback
void GameButton::timerCallback(TimerHandle_t xTimer) {
    // Get back instance using pvTimerGetTimerID
    GameButton* instance = static_cast<GameButton*>(pvTimerGetTimerID(xTimer));
    instance->turnOff();
}