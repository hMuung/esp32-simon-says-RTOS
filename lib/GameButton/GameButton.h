// GameButton.h
#pragma once
#include <Arduino.h>
#include <freertos/FreeRTOS.h>
#include <freertos/task.h>

class GameButton {
private:
    int toneFreq;
    uint8_t buttonPin;
    uint8_t ledPin;

    uint32_t lastInterruptTime;
    TimerHandle_t feedbackOffTimer;

    // Class Constants
    static const uint32_t bounceThreshold = 150;
    static const uint32_t fadeTime = 150;

    static void IRAM_ATTR isrWrapper(void* arg);
    static void timerCallback(TimerHandle_t xTimer);

public:
    // Constructor declaration
    GameButton(uint8_t, uint8_t, int);

    // Shared speakerPin
    static uint8_t sharedSpeakerPin;

    // Method declarations
    void begin();
    void turnOff();
    void IRAM_ATTR handleInterrupt();

};

