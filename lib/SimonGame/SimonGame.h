#pragma once

#include <Arduino.h>
#include <freertos/FreeRTOS.h>
#include <freertos/queue.h>


class SimonGame {
    private:
        QueueHandle_t buttonQueue;
        QueueHandle_t outputQueue;
        QueueHandle_t displayQueue;

        static const int delayBtwSecuence = 200;
        static const int delayBfSecuence = 500;
        static const int delayBfNextLevel = 500;

        static const int maxSequence = 99;
        uint8_t sequence[maxSequence];

        uint8_t level;
        uint8_t inputIndex;

        enum GameState {
            IDLE,
            START_GAME,
            SHOW_SEQUENCE,
            WAIT_INPUT,
            SUCCESS,
            GAME_OVER
        };

        GameState state;

        uint8_t generateNextStep();
        void sendOutput(uint8_t id);
        void sendScore(int score);

    public:
        SimonGame(
            QueueHandle_t buttonQueue,
            QueueHandle_t outputQueue,
            QueueHandle_t displayQueue
        );

        void run();
};