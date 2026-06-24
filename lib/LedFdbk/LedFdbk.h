// LedFdbk.h
#pragma once

#include <Arduino.h>
#include <freertos/FreeRTOS.h>
#include <freertos/timers.h>


class LedFdbk {
    private:
        uint8_t ledPin;

        TimerHandle_t feedbackOffTimer;

        // Class Constants
        static const uint32_t fadeTime = 150;

        static void timerCallback(TimerHandle_t xTimer);
        void timerExpired();

    public:
        // Constructor declaration
        LedFdbk(uint8_t lPin);

        // Method declarations
        void begin();
        void turnOn();
        void turnOff();

};

