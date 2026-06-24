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

        QueueHandle_t eventQueue = nullptr;

        volatile uint32_t lastInterruptTime = 0;
        static constexpr uint32_t bounceThresholdMs = 150;

        // static: required because attachInterruptArg expects a C-style callback
        static void IRAM_ATTR isrWrapper(void* arg);

        // IRAM_ATTR: keeps ISR code in instruction RAM on ESP32
        void IRAM_ATTR handleInterrupt();


    public:
        // explicit: avoids accidental implicit construction
        explicit GameButton(uint8_t btnPin, uint8_t id, QueueHandle_t queue);

        // delete: prevents copying an object attached to a hardware interrupt
        GameButton(const GameButton&) = delete;
        GameButton& operator=(const GameButton&) = delete;

        void begin();
        uint8_t getButtonPin() const;
};

