// GameButton.cpp
#include "GameButton.h"


// Constructor stores fixed button configuration
GameButton::GameButton(uint8_t btnPin, uint8_t id, QueueHandle_t queue)
    : buttonPin(btnPin), buttonId(id), eventQueue(queue) {}


// Initializes button pin and attaches interrupt
void GameButton::begin() {
    pinMode(buttonPin, INPUT_PULLUP);

    attachInterruptArg(
        digitalPinToInterrupt(buttonPin),
        isrWrapper,
        static_cast<void*>(this),
        FALLING
    );
}

// ISR-safe interrupt handler with debounce
void IRAM_ATTR GameButton::handleInterrupt() {
    
    const uint32_t currentTime = millis();

    if ((currentTime - lastInterruptTime) < bounceThresholdMs) {
        return;
    }

    lastInterruptTime = currentTime;

    if (eventQueue == nullptr) {
        return;
    }

    BaseType_t higherPriorityTaskWoken = pdFALSE;

    const uint8_t idToSend = buttonId;

    xQueueSendFromISR( eventQueue, &idToSend, &higherPriorityTaskWoken);

    if (higherPriorityTaskWoken == pdTRUE) {
        portYIELD_FROM_ISR();
    }

}


// Returns the button pin
uint8_t GameButton::getButtonPin() const {
    return buttonPin;
}


// Static ISR wrapper bridges C callback to C++ instance
void IRAM_ATTR GameButton::isrWrapper(void* arg) {
    // Cast argument back to GameButton instance
    auto* instance = static_cast<GameButton*>(arg);

    if (instance != nullptr) {
        instance->handleInterrupt();
    }
}