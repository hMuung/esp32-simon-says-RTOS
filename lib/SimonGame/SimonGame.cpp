#include "SimonGame.h"

SimonGame::SimonGame(
    QueueHandle_t btnQueue,
    QueueHandle_t outQueue,
    QueueHandle_t dispQueue
) :
    buttonQueue(btnQueue),
    outputQueue(outQueue),
    displayQueue(dispQueue),
    idleTimer(nullptr),
    level(0),
    inputIndex(0),
    state(POWER_UP)
{
    randomSeed(millis());

    idleTimer = xTimerCreate(
        "idleTimer",
        pdMS_TO_TICKS(idleTimeout),
        pdFALSE,          // one-shot timer
        (void*)this,
        idleTimerCallback
    );
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

void SimonGame::resetIdleTimer() {
    if (idleTimer != nullptr) {
        xTimerReset(idleTimer, 0);
    }
}

void SimonGame::stopIdleTimer() {
    if (idleTimer != nullptr) {
        xTimerStop(idleTimer, 0);
    }
}

void SimonGame::idleTimerCallback(TimerHandle_t xTimer) {
    SimonGame* instance = static_cast<SimonGame*>(pvTimerGetTimerID(xTimer));

    if (instance != nullptr) {
        instance->setIdleMode();
    }
}

void SimonGame::setIdleMode() {
    xQueueReset(buttonQueue);
    state = IDLE;
}

void SimonGame::overLoadOutput(int times, int delayTime) {
    for (int i = 0; i < times ; i++) {
        if (state == IDLE) break;

        sendOutput(0);
        sendOutput(1);
        sendOutput(2);
        sendOutput(3);
        
        if (delayTime) {
            vTaskDelay(pdMS_TO_TICKS(delayTime));
        }
    }
}

void SimonGame::run() {
    uint8_t pressedButton;

    for (;;) {

        switch (state) {

            case IDLE:
                stopIdleTimer();
                level = 0;
                inputIndex = 0;
                sendScore(idleScore);

                if (xQueueReceive(buttonQueue, &pressedButton, portMAX_DELAY) == pdTRUE) {
                    resetIdleTimer();
                    state = START_GAME;
                }
                break;

            case POWER_UP:
                level = 0;
                inputIndex = 0;
                resetIdleTimer();
                sendScore(gameOverScore);

                if (xQueueReceive(buttonQueue, &pressedButton, portMAX_DELAY) == pdTRUE) {
                    resetIdleTimer();
                    state = START_GAME;
                }
                break;

            case START_GAME:
                resetIdleTimer();

                level = 1;
                inputIndex = 0;
                sequence[0] = generateNextStep();

                sendOutput(0);
                vTaskDelay(pdMS_TO_TICKS(100));
                sendOutput(1);
                vTaskDelay(pdMS_TO_TICKS(100));
                sendOutput(2);
                vTaskDelay(pdMS_TO_TICKS(100));
                sendOutput(3);
                vTaskDelay(pdMS_TO_TICKS(delayBfNextLevel));

                state = SHOW_SEQUENCE;
                break;

            case SHOW_SEQUENCE:
                resetIdleTimer();

                vTaskDelay(pdMS_TO_TICKS(delayBfSecuence));

                if (state == IDLE) break;

                sendScore(idleScore);

                for (int i = 0; i < level; i++) {
                    if (state == IDLE) break;

                    sendOutput(sequence[i]);
                    vTaskDelay(pdMS_TO_TICKS(delayBtwSecuence));
                }

                if (state != IDLE) {
                    inputIndex = 0;
                    state = WAIT_INPUT;
                }

                xQueueReset(buttonQueue);

                break;

            case WAIT_INPUT:
                if (xQueueReceive(buttonQueue, &pressedButton,pdMS_TO_TICKS(100)) == pdTRUE) {

                    resetIdleTimer();

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
                resetIdleTimer();

                level++;

                if (level >= maxSequence) {
                    level = maxSequence;
                }

                sequence[level - 1] = generateNextStep();

                if (state != IDLE) {
                    vTaskDelay(pdMS_TO_TICKS(delayBfNextLevel));
                }

                sendScore(level);

                overLoadOutput(blinkTimesBfNext,delayBtwBlinks);

                if (state != IDLE) {
                    vTaskDelay(pdMS_TO_TICKS(delayBfNextLevel));
                    state = SHOW_SEQUENCE;
                }

                break;

            case GAME_OVER:
                resetIdleTimer();

                overLoadOutput(3,300);

                sendScore(gameOverScore);
                state = IDLE;
                break;
        }
    }
}