// LedSound.h
#pragma once

#include <Arduino.h>
#include <freertos/FreeRTOS.h>
#include <freertos/timers.h>


class LedSound {
    private:
        int toneFreq;
        uint8_t ledPin;

        TimerHandle_t feedbackOffTimer;

        // Class Constants
        static const uint32_t fadeTime = 150;

        static void timerCallback(TimerHandle_t xTimer);

    public:
        // Constructor declaration
        LedSound(uint8_t lPin, int freq);

        // Shared speakerPin
        static uint8_t sharedSpeakerPin;

        // Method declarations
        void begin();
        void turnOn();
        void turnOff();

};

