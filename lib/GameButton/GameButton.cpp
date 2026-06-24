// GameButton.cpp
#include "GameButton.h"

// Class Constructor
GameButton::GameButton(uint8_t btnPin, uint8_t id, QueueHandle_t queue)
    : buttonPin(btnPin), buttonId(id), eventQueue(queue), lastInterruptTime(0) {}


// Initialize method
void GameButton::begin() {

    // Pin mode declaration
    pinMode(buttonPin,INPUT_PULLUP);

    // Attacht interrupt
    attachInterruptArg(
        digitalPinToInterrupt(buttonPin),
        isrWrapper,
        this,// argument to the isrWrapper
        FALLING
    );

}

// Interrupt handeling
void IRAM_ATTR GameButton::handleInterrupt() {
    
    uint32_t currentTime = millis(); 

    // Bounce software handling
    if ((currentTime - lastInterruptTime) >= bounceThreshold) {
        lastInterruptTime = currentTime;
        
        // Verify if Queue exist
        if (eventQueue != nullptr) {
            BaseType_t xHigherPriorityTaskWoken = pdFALSE;
            
            // Send button ID to interrupt Queue
            xQueueSendFromISR(eventQueue, &buttonId, &xHigherPriorityTaskWoken);
            
            // If the task that awaited the queue has higher priority, forze context change
            if (xHigherPriorityTaskWoken) {
                portYIELD_FROM_ISR();
            }
        }
    }

}

// Return buttons Pin
uint8_t GameButton::getButtonPin() {
    return buttonPin;
}

// ISR Wrapper
void IRAM_ATTR GameButton::isrWrapper(void* arg) {
    // Cast argument back to GameButton instance
    GameButton* instance = static_cast<GameButton*>(arg);
    instance->handleInterrupt();
}
