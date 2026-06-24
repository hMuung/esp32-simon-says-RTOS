// Display.h
#pragma once

/*
Module responsibility:
    Drives two 7-segment displays using shift registers.
    It can show numbers from 0 to 99, dashes, or turn the display off.
*/

#include <Arduino.h>


class Display {

    private:

        const uint8_t latchPin;
        const uint8_t dataPin;
        const uint8_t clockPin;

        int currentNumber = DisplayOff;

        static constexpr int DisplayOff = -1;
        static constexpr int DisplayDash = -2;

        static constexpr uint8_t off = 0b00000000;
        static constexpr uint8_t dash = 0b01000000;
        static constexpr uint8_t digitTable[10] = {
            0b00111111, // 0
            0b00000110, // 1
            0b01011011, // 2
            0b01001111, // 3
            0b01100110, // 4
            0b01101101, // 5
            0b01111101, // 6
            0b00000111, // 7
            0b01111111, // 8
            0b01101111  // 9
        };

        // Sends two bytes to the shift registers
        void writeRegister(uint8_t highDigitByte, uint8_t lowDigitByte);

        
    public:

        // explicit: avoids accidental implicit conversions
        explicit Display(uint8_t ltcPin, uint8_t dtPin, uint8_t clkPin);

        void begin();

        // const: does not modify object state
        int getDisplayedNumber() const;

        void showNumber(int number);
        void showDash();
        void turnOff();
};