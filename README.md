# Simon Game ESP32-C3

Classic Simon memory game implemented on an ESP32-C3 using C++, Arduino Framework and FreeRTOS,

developed with PlatformIO in Visual Studio Code and tested using the Wokwi online simulator.

## Features
- Object-oriented C++ architecture
- FreeRTOS multitasking
- Queue-based communication between tasks
- GPIO interrupts for button handling
- Software debounce
- FreeRTOS timers
- State machine game control
- LED, sound and display feedback

## Hardware
- ESP32-C3 DevKitM-1
- 4 Push buttons
- 4 LEDs
- Passive buzzer
- Two 7-segment displays
- 2 Shift registers 74hc595
- Four 220 ohms resistors

## Software
- PlatformIO
- Arduino Framework
- FreeRTOS
- Wokwi Simulator

## Architecture
The firmware is divided into independent modules:

```
SimonGame
    Game logic
    State machine
    Sequence validation

GameButton
    GPIO interrupts
    Debounce
    Button events

LedFdbk
    LED feedback timers

SoundFdbk
    Buzzer control

Display
    7-segment display driver
```

## FreeRTOS Tasks
```
gameTask
    |
    +-- ledQueue ------> LED Task
    |
    +-- soundQueue ----> Sound Task
    |
    +-- displayQueue --> Display Task


Buttons
    |
buttonQueue
    |
SimonGame
```

## Online Simulation

Run the project directly using Wokwi:

https://wokwi.com/projects/467757161253660673

Feel free to comment
