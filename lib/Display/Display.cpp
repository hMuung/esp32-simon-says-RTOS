#include "Display.h"

// Requiered definition for the linker
constexpr uint8_t Display::digitTable[10];


// Constructor stores fixed hardware pins
Display::Display(uint8_t ltcPin, uint8_t dtPin, uint8_t clkPin)
    : latchPin(ltcPin), dataPin(dtPin), clockPin(clkPin) {}


// Initializes shift-register control pins
void Display::begin() {
    pinMode(latchPin, OUTPUT);
    pinMode(dataPin, OUTPUT);
    pinMode(clockPin, OUTPUT);
    turnOff();
}


// Returns the current display state
int Display::getDisplayedNumber() const {
    return currentNumber;
}


// Shows a number from 0 to 99
void Display::showNumber(int number) {

    if (number > 99) {
        return;
    }
    
    if (number == -1) {
        turnOff();
        return;
    } 
    
    if (number == -2) {
        showDash();
        return;
    }

    currentNumber = number;

    const uint8_t highDigit = number / 10;
    const uint8_t lowDigit = number % 10;

    writeRegister(digitTable[highDigit], digitTable[lowDigit]);
}


// Shows dash symbols on both digits
void Display::showDash() {
    currentNumber = DisplayDash;
    writeRegister(dash, dash);
}


// Turns both digits off
void Display::turnOff() {
    currentNumber = DisplayOff;
    writeRegister(off, off);
}


// Writes both digits to the shift registers
void Display::writeRegister(uint8_t highDigitByte, uint8_t lowDigitByte) {
    digitalWrite(latchPin, LOW);
    shiftOut(dataPin, clockPin, MSBFIRST, lowDigitByte);
    shiftOut(dataPin, clockPin, MSBFIRST, highDigitByte);
    digitalWrite(latchPin, HIGH);
}