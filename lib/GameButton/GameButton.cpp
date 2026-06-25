// GameButton.cpp
#include "GameButton.h"


// Constructor stores fixed button configuration
GameButton::GameButton(
    uint8_t btnPin, 
    uint8_t id, 
    int freq, 
    QueueHandle_t btnQueue, 
    QueueHandle_t ldQueue, 
    QueueHandle_t sndQueue 
) : 
buttonPin(btnPin), 
buttonId(id), 
frequency(freq),
buttonQueue(btnQueue),
ledQueue(ldQueue),
soundQueue(sndQueue) 
{}


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

    if (ledQueue == nullptr || soundQueue == nullptr || buttonQueue == nullptr) {
        return;
    }

    BaseType_t higherPriorityTaskWoken = pdFALSE;

    const uint8_t idToSend = buttonId;
    const int freqToSend = frequency;

    xQueueSendFromISR( buttonQueue, &idToSend, &higherPriorityTaskWoken);
    xQueueSendFromISR( ledQueue, &idToSend, &higherPriorityTaskWoken);
    xQueueSendFromISR( soundQueue, &freqToSend, &higherPriorityTaskWoken);

    if (higherPriorityTaskWoken == pdTRUE) {
        portYIELD_FROM_ISR();
    }

}

// Updates the event queue before attaching interrupt
void GameButton::setQueues(QueueHandle_t btnQueue, QueueHandle_t ldQueue, QueueHandle_t sndQueue) {
    buttonQueue = btnQueue;
    ledQueue = ldQueue;
    soundQueue = sndQueue;
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