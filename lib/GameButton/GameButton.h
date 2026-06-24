// GameButton.h
#pragma once

#include <Arduino.h>
#include <freertos/FreeRTOS.h>
#include <freertos/queue.h>


class GameButton {
    private:
        uint8_t buttonPin;
        uint8_t buttonId;

        // Comunication Queue
        QueueHandle_t eventQueue;

        // Debounce time
        uint32_t lastInterruptTime;

        // Class Constants
        static const uint32_t bounceThreshold = 150;

        static void IRAM_ATTR isrWrapper(void* arg);

    public:
        // Constructor declaration
        GameButton(uint8_t btnPin, uint8_t id, QueueHandle_t queue);

        // Method declarations
        void begin();
        uint8_t getButtonPin();
        void IRAM_ATTR handleInterrupt();

};

