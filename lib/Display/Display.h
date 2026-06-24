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

        static constexpr uint8_t off = 0b11111111;
        static constexpr uint8_t dash = 0b10111111;
        static constexpr uint8_t digitTable[10] = {
            0b11000000, 0b11111001, 0b10100100, 0b10110000, 0b10011001,
            0b10010010, 0b10000010, 0b11111000, 0b10000000, 0b10010000,
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