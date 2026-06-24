#include "SimonGame.h"

// Requiered definition for the linker
constexpr int SimonGame::gameTones[4];

SimonGame::SimonGame(
    QueueHandle_t btnQueue,
    QueueHandle_t ldQueue, 
    QueueHandle_t sndQueue,
    QueueHandle_t dspQueue
) :
    buttonQueue(btnQueue),
    ledQueue(ldQueue),
    soundQueue(sndQueue),
    displayQueue(dspQueue),
    idleTimer(nullptr),
    level(0),
    inputIndex(0),
    state(POWER_UP)
{   
    // Idle timer
    idleTimer = xTimerCreate(
        "idleTimer",
        pdMS_TO_TICKS(idleTimeout),
        pdFALSE,          // one-shot timer
        (void*)this,
        idleTimerCallback
    );

    resetIdleTimer();

}


uint8_t SimonGame::generateNextStep() {
    return esp_random() % 4;
}

void SimonGame::sendScore(int score) {
    xQueueSend( displayQueue, &score, portMAX_DELAY);
}

void SimonGame::sendLed(uint8_t id) {
    xQueueSend( ledQueue, &id, portMAX_DELAY);
}

void SimonGame::sendSound(int freq) {
    xQueueSend( soundQueue, &freq, portMAX_DELAY);
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

void SimonGame::turnOnAllLeds() {
    for (uint8_t i = 0; i < 4; i++) {
        sendLed(i);
    }
}

// State machine function
void SimonGame::run() {
    for (;;) {
        switch (state) {
            case IDLE:
                handleIdle();
                break;

            case POWER_UP:
                handlePowerUp();
                break;

            case START_GAME:
                handleStartGame();
                break;

            case SHOW_SEQUENCE:
                handleShowSequence();
                break;

            case WAIT_INPUT:
                handleWaitInput();
                break;

            case SUCCESS:
                handleSuccess();
                break;

            case GAME_OVER:
                handleGameOver();
                break;
        }
    }
}

// State handlers

void SimonGame::handleIdle() {
    uint8_t pressedButton;

    stopIdleTimer();
    level = 0;
    inputIndex = 0;
    sendScore(idleScore);

    if (xQueueReceive(buttonQueue, &pressedButton, portMAX_DELAY) == pdTRUE) {
        resetIdleTimer();
        state = START_GAME;
    }
}

void SimonGame::handlePowerUp() {
    uint8_t pressedButton;

    level = 0;
    inputIndex = 0;
    sendScore(gameOverScore);

    if (xQueueReceive(buttonQueue, &pressedButton, pdMS_TO_TICKS(100)) == pdTRUE) {
        resetIdleTimer();
        state = START_GAME;
    }
}

void SimonGame::handleStartGame() {
    resetIdleTimer();

    level = 1;
    inputIndex = 0;
    sequence[0] = generateNextStep();
  
    for (uint8_t i = 0; i < 4; i++) {
        sendLed(i);
        sendSound(gameTones[i]);
        vTaskDelay(pdMS_TO_TICKS(100));
    }

    vTaskDelay(pdMS_TO_TICKS(delayBfNextLevel));

    state = SHOW_SEQUENCE;

}

void SimonGame::handleShowSequence() {
    uint8_t nextStep{};

    resetIdleTimer();

    vTaskDelay(pdMS_TO_TICKS(delayBfSecuence));

    if (state == IDLE) return;

    sendScore(level);

    for (uint8_t i = 0; i < level; i++) {
        if (state == IDLE) return;
        nextStep = sequence[i];
        sendLed(nextStep);
        sendSound(gameTones[nextStep]);
        vTaskDelay(pdMS_TO_TICKS(delayBtwSecuence));
    }
    
    vTaskDelay(pdMS_TO_TICKS(delayBtwSecuence));

    if (state != IDLE) {
        inputIndex = 0;
        state = WAIT_INPUT;
    }

    xQueueReset(buttonQueue);
}

void SimonGame::handleWaitInput() {
    uint8_t pressedButton;

    if (xQueueReceive(buttonQueue, &pressedButton,pdMS_TO_TICKS(100)) == pdTRUE) {

        resetIdleTimer();

        sendLed(pressedButton);
        sendSound(gameTones[pressedButton]);

        if (pressedButton == sequence[inputIndex]) {
            inputIndex++;

            if (inputIndex >= level) {
                state = SUCCESS;
            }

        } else {
            state = GAME_OVER;
        }
    }
}

void SimonGame::handleSuccess() {
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

    turnOnAllLeds();

    for (uint8_t i = 0; i < 4; i++) {
        sendSound(gameTones[i]);
        pdMS_TO_TICKS(50);
    }

    if (state != IDLE) {
        vTaskDelay(pdMS_TO_TICKS(delayBfNextLevel));
        state = SHOW_SEQUENCE;
    }
}

void SimonGame::handleGameOver() {
    resetIdleTimer();

    turnOnAllLeds();
    sendSound(NOTE_DS5);
    vTaskDelay(pdMS_TO_TICKS(150));

    turnOnAllLeds();
    sendSound(NOTE_DS5);
    vTaskDelay(pdMS_TO_TICKS(150));

    turnOnAllLeds();
    sendSound(NOTE_CS5);
    vTaskDelay(pdMS_TO_TICKS(150));

    turnOnAllLeds();
    for (uint8_t i = 0; i < 5; i++) {
        for (int pitch = -10; pitch <= 10; pitch++) {
        sendSound(NOTE_C5 + pitch);
        vTaskDelay(pdMS_TO_TICKS(6));
        }
    }
    
    state = POWER_UP;
    
}



