// GameButton.h
#pragma once

/*
Module responsibility:
    Handles a physical game button using an interrupt,
    it debounces the signal and sends the button ID to a FreeRTOS queue.
*/

#include <Arduino.h>
#include <freertos/FreeRTOS.h>
#include <freertos/queue.h>


class GameButton {

    private:

        const uint8_t buttonPin;
        const uint8_t buttonId;
        const int frequency;

        QueueHandle_t buttonQueue = nullptr;
        QueueHandle_t ledQueue = nullptr;
        QueueHandle_t soundQueue = nullptr;

        volatile uint32_t lastInterruptTime = 0;
        static constexpr uint32_t bounceThresholdMs = 150;

        // static: required because attachInterruptArg expects a C-style callback
        static void IRAM_ATTR isrWrapper(void* arg);

        // IRAM_ATTR: keeps ISR code in instruction RAM on ESP32
        void IRAM_ATTR handleInterrupt();


    public:
        
        GameButton(
            uint8_t btnPin,
            uint8_t id,
            int freq,
            QueueHandle_t buttonQueue = nullptr,
            QueueHandle_t ldQueue = nullptr,
            QueueHandle_t sndQueue = nullptr
        );

        // delete: prevents copying an object attached to a hardware interrupt
        GameButton(const GameButton&) = delete;
        GameButton& operator=(const GameButton&) = delete;

        void begin();
        void setQueues(QueueHandle_t btnQueue, QueueHandle_t ldQueue, QueueHandle_t sndQueue);
        uint8_t getButtonPin() const;
};

