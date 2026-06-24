#include <Arduino.h>
#include <freertos/FreeRTOS.h>
#include <freertos/task.h>
#include <freertos/queue.h>

#include "pitches.h"
#include "GameButton.h"
#include "LedSound.h"
#include "Display.h"
#include "SimonGame.h"

QueueHandle_t buttonQueue;
QueueHandle_t outputQueue;
QueueHandle_t displayQueue;

Display display(7, 5, 6);

LedSound outputs[] = {
    LedSound(18, NOTE_G3),
    LedSound(8,  NOTE_C4),
    LedSound(19, NOTE_E4),
    LedSound(9,  NOTE_G4)
};

GameButton buttons[] = {
    GameButton(1,  0, nullptr),
    GameButton(3,  1, nullptr),
    GameButton(10, 2, nullptr),
    GameButton(2,  3, nullptr)
};

SimonGame* game;

void gameTask(void* pvParameters) {
    game->run();
}

void outputTask(void* pvParameters) {
    uint8_t id{};

    for (;;) {
        if (xQueueReceive(outputQueue, &id, portMAX_DELAY) == pdTRUE) {
            if (id < 4) {
                outputs[id].turnOn();
            }
        }
    }
}

void displayTask(void* pvParameters) {
    int score{};

    for (;;) {
        if (xQueueReceive(displayQueue, &score, portMAX_DELAY) == pdTRUE) {
            display.showNumber(score);
        }
    }
}

void setup() {

    LedSound::sharedSpeakerPin = 0;

    buttonQueue = xQueueCreate(10, sizeof(uint8_t));
    outputQueue = xQueueCreate(10, sizeof(uint8_t));
    displayQueue = xQueueCreate(5, sizeof(int));

    display.begin();
    display.showDash();

    for (int i = 0; i < 4; i++) {
        buttons[i] = GameButton(buttons[i].getButtonPin(), i, buttonQueue);
        buttons[i].begin();

        outputs[i].begin();
    }

    game = new SimonGame(buttonQueue, outputQueue, displayQueue);

    xTaskCreate(gameTask, "GameTask", 4096, NULL, 2, NULL);
    xTaskCreate(outputTask, "OutputTask", 2048, NULL, 1, NULL);
    xTaskCreate(displayTask, "DisplayTask", 2048, NULL, 1, NULL);
}

void loop() {}