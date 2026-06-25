#pragma once

#include <Arduino.h>
#include <freertos/FreeRTOS.h>
#include <freertos/queue.h>
#include <freertos/timers.h>
#include <esp_random.h>

#include <pitches.h>

class SimonGame {
private:
    QueueHandle_t buttonQueue;
    QueueHandle_t ledQueue;
    QueueHandle_t soundQueue;
    QueueHandle_t displayQueue;

    TimerHandle_t idleTimer;

    static constexpr int gameTones[4] = {
        NOTE_G3,
        NOTE_C4,
        NOTE_E4,
        NOTE_G4
    };

    static const int idleTimeout = 10000;
    static const int delayBtwSecuence = 200;
    static const int delayBfSecuence = 500;
    static const int delayBfNextLevel = 500;

    static const int idleScore = -1;
    static const int gameOverScore = -2;

    static const int maxSequence = 99;
    uint8_t sequence[maxSequence];

    uint8_t level;
    uint8_t inputIndex;

    enum GameState {
        IDLE,
        POWER_UP,
        START_GAME,
        SHOW_SEQUENCE,
        WAIT_INPUT,
        SUCCESS,
        GAME_OVER
    };

    volatile GameState state;

    uint8_t generateNextStep();

    void sendLed(uint8_t id);
    void sendSound(int freq);
    void sendScore(int score);
    void turnOnAllLeds();

    void resetIdleTimer();
    void stopIdleTimer();
    void setIdleMode();

    void handleIdle();
    void handlePowerUp();
    void handleStartGame();
    void handleShowSequence();
    void handleWaitInput();
    void handleSuccess();
    void handleGameOver();

    static void idleTimerCallback(TimerHandle_t xTimer);

public:
    SimonGame(
        QueueHandle_t btnQueue,
        QueueHandle_t ldQueue, 
        QueueHandle_t sndQueue,
        QueueHandle_t dspQueue
    );

    void run();
};