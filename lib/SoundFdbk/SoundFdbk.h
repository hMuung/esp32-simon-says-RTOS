// SoundFdbk.h
#pragma once

#include <Arduino.h>
#include <freertos/FreeRTOS.h>
#include <freertos/timers.h>


class SoundFdbk {
    private:
        uint8_t buzzerPin;

        TimerHandle_t feedbackOffTimer;

        // Class Constants
        static const uint32_t fadeTime = 150;

        static void timerCallback(TimerHandle_t xTimer);
        void timerExpired();

    public:
        // Constructor declaration
        SoundFdbk(uint8_t bzrPin);

        // Method declarations
        void begin();
        void playSound(int freq);
        void stopSound();

};

