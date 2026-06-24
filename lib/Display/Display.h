#pragma once

#include <Arduino.h>

class Display {
    private:
        
        // Pin output
        const uint8_t latchPin;
        const uint8_t dataPin;
        const uint8_t clockPin;

        int currentNumber = 0;

        // Digit table for the 7-segment display
        static constexpr uint8_t digitTable[10] = {
            0b11000000, 0b11111001, 0b10100100, 0b10110000,
            0b10011001, 0b10010010, 0b10000010, 0b11111000,
            0b10000000, 0b10010000
        };
        static constexpr uint8_t dash = 0b10111111;
        static constexpr uint8_t off = 0b11111111;

        void writeRegister(uint8_t highDigitByte, uint8_t lowDigitByte);

    public:
        // Constructor
        Display(uint8_t ltcPin, uint8_t dtPin, uint8_t clkPin);

        // Methods
        void begin();
        void showNumber(int number);
        void showDash();
        void turnOff();
        int getDisplayedNumber();
};
