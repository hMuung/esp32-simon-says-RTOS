// LedFdbk.h
#pragma once

/* 
Module responsibility:
    Provides short LED feedback pulses using a FreeRTOS one-shot timer,
    the class owns the LED pin and its timer
*/

#include <Arduino.h>
#include <freertos/FreeRTOS.h>
#include <freertos/timers.h>


class LedFdbk {

    private:
        
        const uint8_t ledPin;

        TimerHandle_t feedbackOffTimer = nullptr;
        static constexpr TickType_t fadeTimeMs = 150;

        void setLed(bool state);

        void timerExpired();
        static void timerCallback(TimerHandle_t xTimer);


    public:

        // explicit: avoids accidental implicit conversions from uint8_t
        explicit LedFdbk(uint8_t lPin);

        // Destructor releases the FreeRTOS timer
        ~LedFdbk();

        // delete: prevent copying a class that owns a timer handle
        LedFdbk(const LedFdbk&) = delete;
        LedFdbk& operator=(const LedFdbk&) = delete;

        void begin();
        void turnOn();
        void turnOff();
};