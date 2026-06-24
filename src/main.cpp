#include <Arduino.h>
#include <freertos/FreeRTOS.h>
#include <freertos/task.h>
#include <freertos/queue.h>

#include "pitches.h"
#include "GameButton.h"
#include "LedFdbk.h"
#include "SoundFdbk.h"
#include "Display.h"
#include "SimonGame.h"


QueueHandle_t buttonQueue;
QueueHandle_t ledQueue;
QueueHandle_t soundQueue;
QueueHandle_t displayQueue;


Display display(7, 5, 6);
SoundFdbk buzzer(0);

LedFdbk leds[] {{18}, {8}, {19}, {9}};

GameButton buttons[] {
    {1, 0, NOTE_G3},
    {3, 1, NOTE_C4},
    {10, 2, NOTE_E4},
    {2, 3, NOTE_G4},
};

SimonGame* game = nullptr;

// Task prototipes
void gameTask(void* pvParameters);
void ledTask(void* pvParameters) ;
void soundTask(void* pvParameters);
void displayTask(void* pvParameters);


// Initialization
void setup() {

    buttonQueue = xQueueCreate(10, sizeof(uint8_t));
    ledQueue = xQueueCreate(10, sizeof(uint8_t));
    soundQueue = xQueueCreate(10, sizeof(int));
    displayQueue = xQueueCreate(5, sizeof(int));

    buzzer.begin();
    display.begin();
    display.showDash();

    // Led and Buttons initialization
    for (int i = 0; i < 4; i++) {
        buttons[i].setQueues(buttonQueue,ledQueue,soundQueue);
        buttons[i].begin();

        leds[i].begin();
    }

    game = new SimonGame(buttonQueue, ledQueue, soundQueue, displayQueue); 

    xTaskCreate(gameTask, "gameTask", 4096, NULL, 2, NULL);
    xTaskCreate(ledTask, "ledTask", 2048, NULL, 1, NULL);
    xTaskCreate(soundTask, "soundTask", 2048, NULL, 1, NULL);
    xTaskCreate(displayTask, "DisplayTask", 2048, NULL, 1, NULL);
}


void loop() {}

// Game logic task
void gameTask(void* pvParameters) {
    game->run();
}

// Led turn on-off task
void ledTask(void* pvParameters) {
    uint8_t id{};

    for (;;) {
        if (xQueueReceive(ledQueue, &id, portMAX_DELAY) == pdTRUE) {
            if (id < 4) {
                leds[id].turnOn();
            }
        }
    }
}

// Sound on-off task
void soundTask(void* pvParameters) {
    int freq{};

    for (;;) {
        if (xQueueReceive(soundQueue, &freq, portMAX_DELAY) == pdTRUE) {
                buzzer.playSound(freq);
        }
    }
}

// Display task
void displayTask(void* pvParameters) {
    int score{};

    for (;;) {
        if (xQueueReceive(displayQueue, &score, portMAX_DELAY) == pdTRUE) {
            display.showNumber(score);
        }
    }
}