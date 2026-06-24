#include <Arduino.h>
#include <freertos/FreeRTOS.h>
#include <freertos/task.h>
#include <freertos/queue.h>

#include "pitches.h"
#include "GameButton.h"
#include "LedSound.h"
#include "Display.h"

// FreeRTOS variables
QueueHandle_t buttonQueue;
TaskHandle_t testTaskHandle;

// Display object
Display display(7, 5, 6); // Latch, Data, Clock

// Output arrays (LEDs and sound)
LedSound outputs[] = {
    LedSound(18, NOTE_G3), // ID 0
    LedSound(8,  NOTE_C4), // ID 1
    LedSound(19, NOTE_E4), // ID 2
    LedSound(9,  NOTE_G4)  // ID 3
};

// Input arrays (buttons) queue is assigned on setup
GameButton buttons[] = {
    GameButton(1,  0, nullptr), 
    GameButton(3,  1, nullptr), 
    GameButton(10, 2, nullptr), 
    GameButton(2,  3, nullptr)  
};

// Test task
void testTask(void *pvParameters) {
    uint8_t pressedButtonId;
    
    for (;;) {
        // Waits for Queue
        if (xQueueReceive(buttonQueue, &pressedButtonId, portMAX_DELAY) == pdTRUE) {
            
            // Turn on led and sound
            outputs[pressedButtonId].turnOn();
        
            // Show ID on display
            display.showNumber(pressedButtonId);
        }
    }
}

// Initial setup
void setup() {

    // Initialize buzzer pin
    LedSound::sharedSpeakerPin = 0;

    // Build freeRTOS queue with capaciti for 10 elements and size of 1 byte for ID
    buttonQueue = xQueueCreate(10, sizeof(uint8_t));

    // Re-assing queue to bottons and initialize hardware
    for (int i = 0; i < 4; i++) {
        buttons[i] = GameButton(buttons[i].getButtonPin(), i, buttonQueue);
        buttons[i].begin();
        outputs[i].begin();
    }

    display.begin();
    display.showDash();

    // Build test task
    xTaskCreate(
        testTask,          
        "TestLogicTask",   
        2048,              
        NULL,              
        1,                 
        &testTaskHandle    
    );
}


void loop() {}