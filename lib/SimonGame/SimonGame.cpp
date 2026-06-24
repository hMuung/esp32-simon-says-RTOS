#include "SimonGame.h"

SimonGame::SimonGame(QueueHandle_t btnQueue, QueueHandle_t 
    outQueue, QueueHandle_t dispQueue) : buttonQueue(btnQueue), 
    outputQueue(outQueue), displayQueue(dispQueue), level(0),
    inputIndex(0), state(IDLE) 
{
    randomSeed(millis());
}

uint8_t SimonGame::generateNextStep() {
    return random(0, 4);
}

void SimonGame::sendOutput(uint8_t id) {
    xQueueSend(outputQueue, &id, portMAX_DELAY);
}

void SimonGame::sendScore(int score) {
    xQueueSend(displayQueue, &score, portMAX_DELAY);
}

void SimonGame::run() {
    uint8_t pressedButton;

    for (;;) {
        switch (state) {

            case IDLE:
                sendScore(-1);

                if (xQueueReceive(buttonQueue, &pressedButton, portMAX_DELAY) == pdTRUE) {
                    state = START_GAME;
                }
                break;

            case START_GAME:
                level = 1;
                inputIndex = 0;
                sequence[0] = generateNextStep();
                sendScore(level);
                state = SHOW_SEQUENCE;
                break;

            case SHOW_SEQUENCE:
                vTaskDelay(pdMS_TO_TICKS(delayBfSecuence));

                for (int i = 0; i < level; i++) {
                    sendOutput(sequence[i]);
                    vTaskDelay(pdMS_TO_TICKS(delayBtwSecuence));
                }

                inputIndex = 0;
                state = WAIT_INPUT;
                break;

            case WAIT_INPUT:
                if (xQueueReceive(buttonQueue, &pressedButton, portMAX_DELAY) == pdTRUE) {
                    sendOutput(pressedButton);

                    if (pressedButton == sequence[inputIndex]) {
                        inputIndex++;

                        if (inputIndex >= level) {
                            state = SUCCESS;
                        }
                    } else {
                        state = GAME_OVER;
                    }
                }
                break;

            case SUCCESS:
                level++;

                if (level >= maxSequence) {
                    level = maxSequence;
                }

                sequence[level - 1] = generateNextStep();
                sendScore(level);
                vTaskDelay(pdMS_TO_TICKS(delayBfNextLevel));
                state = SHOW_SEQUENCE;
                break;

            case GAME_OVER:

                for (int i = 0; i < 3; i++) {
                    sendOutput(0);
                    sendOutput(1);
                    sendOutput(2);
                    sendOutput(3);
                    vTaskDelay(pdMS_TO_TICKS(300));
                }

                sendScore(-1);

                state = IDLE;
                break;
        }
    }
}