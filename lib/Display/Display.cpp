#include "Display.h"

// Requiered definition for the linker
constexpr uint8_t Display::digitTable[10];

// Constructor using member initializer list
Display::Display(uint8_t ltcPin, uint8_t dtPin, uint8_t clkPin) :
    latchPin(ltcPin), dataPin(dtPin), clockPin(clkPin) {}

// Initialize hardware pins
void Display::begin() {
    pinMode(latchPin, OUTPUT);
    pinMode(dataPin, OUTPUT);
    pinMode(clockPin, OUTPUT);
}

int Display::getDisplayedNumber() {
    return currentNumber;
}

void Display::showNumber(int number) {

    if (number == -1) {
        showDash();
        return;
    }

    if (number == -2) {
        turnOff();
        return;
    }

    // Bounds check (0-99)
    if (number < 0 || number > 99) {
        return;
    }

    currentNumber = number;
    
    // Splitting digits
    uint8_t highDigit = number / 10;
    uint8_t lowDigit = number % 10;

    writeRegister(digitTable[highDigit], digitTable[lowDigit]);
}

void Display::showDash() {
    currentNumber = -2; // Indicate dash
    writeRegister(dash, dash);
}

void Display::turnOff() {
    currentNumber = -1; // Indicate off
    writeRegister(off, off);
}

void Display::writeRegister(uint8_t highDigitByte, uint8_t lowDigitByte) {
    digitalWrite(latchPin, LOW);
    shiftOut(dataPin, clockPin, MSBFIRST, lowDigitByte);  
    shiftOut(dataPin, clockPin, MSBFIRST, highDigitByte); 
    digitalWrite(latchPin, HIGH);
}
