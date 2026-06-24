#include <Arduino.h>
#include <freertos/FreeRTOS.h>
#include <freertos/task.h>

#include <pitches.h>
#include <GameButton.h>
#include <Display.h>

// GameButton object array
GameButton buttons[] = {
    GameButton(1,  18, NOTE_G3),
    GameButton(3,  8,  NOTE_C4),
    GameButton(10, 19, NOTE_E4),
    GameButton(2,  9,  NOTE_G4)
};

// Dispaly object
Display display(7,5,6);

void setup() {

    // Buzz pin initialization
    GameButton::sharedSpeakerPin = 0;
    tone(GameButton::sharedSpeakerPin, 440, 1); 

    // Initialize game buttons with their leds
    for (GameButton &button : buttons) {
        button.begin();
    }

    // Initialize the two dispalys
    display.begin();
    display.showDash();

}

void loop() {}