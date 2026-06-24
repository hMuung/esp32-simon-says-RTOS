#pragma once

#include <Arduino.h>
#include <freertos/FreeRTOS.h>
#include <freertos/queue.h>
#include <freertos/timers.h>

class SimonGame {
private:
    QueueHandle_t buttonQueue;
    QueueHandle_t outputQueue;
    QueueHandle_t displayQueue;

    TimerHandle_t idleTimer;

    static const int idleTimeout = 10000;
    static const int delayBtwSecuence = 200;
    static const int delayBfSecuence = 500;
    static const int delayBtwBlinks = 100;
    static const int blinkTimesBfNext = 2;
    static const int delayBfNextLevel = 500;

    static const int idleScore = -2;
    static const int gameOverScore = -1;

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

    void sendOutput(uint8_t id);
    void overLoadOutput(int times, int delayTime);
    void sendScore(int score);

    void resetIdleTimer();
    void stopIdleTimer();
    void setIdleMode();

    static void idleTimerCallback(TimerHandle_t xTimer);

public:
    SimonGame(
        QueueHandle_t buttonQueue,
        QueueHandle_t outputQueue,
        QueueHandle_t displayQueue
    );

    void run();
};