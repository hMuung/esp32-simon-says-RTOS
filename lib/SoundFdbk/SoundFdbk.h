// SoundFdbk.h
#pragma once

/*
Module responsibility:
    Provides short buzzer feedback tones using a FreeRTOS one-shot timer,
    the class owns the buzzer pin and its timer
*/

#include <Arduino.h>
#include <freertos/FreeRTOS.h>
#include <freertos/timers.h>


class SoundFdbk {

    private:

        const uint8_t buzzerPin;
        
        TimerHandle_t feedbackOffTimer = nullptr;
        static constexpr TickType_t fadeTimeMs = 150;

        void timerExpired();
        static void timerCallback(TimerHandle_t xTimer);


    public:

        // explicit: avoids accidental implicit conversions from uint8_t
        explicit SoundFdbk(uint8_t bzrPin);

        // Destructor releases the FreeRTOS timer
        ~SoundFdbk();

        // delete: prevent copying a class that owns a timer handle
        SoundFdbk(const SoundFdbk&) = delete;
        SoundFdbk& operator=(const SoundFdbk&) = delete;

        void begin();
        void playSound(int freq);
        void stopSound();
};