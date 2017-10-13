/*
Name:		MidiMixerMono.ino
Created:	4/1/2017 7:18:03 PM
Author:	Jordan Guzak
*/

#include "HardwareIO.h"

#include "DisplayHandler.h"

#include "RotaryHandler.h"
#include "ButtonHandler.h"
#include "HardwareHandler.h"

#include "IsrRoutines.h"


void setup() {
    // for midi communication
    Serial.begin(31250);
    // for testing only
    //Serial.begin(9600);

    pinMode(shiftPin, INPUT_PULLUP);
    pinMode(shiftEncoderState[0][0], INPUT_PULLUP);
    pinMode(shiftEncoderState[0][1], INPUT_PULLUP);

    // only done for first page because each page uses
    //  the same collection of pins.
    for (int i = 0; i < pageButtonEncoders; i++) {
        pinMode(page1EncoderState[i][0], INPUT_PULLUP);
        pinMode(page1EncoderState[i][1], INPUT_PULLUP);

        pinMode(page1ButtonState[i][0], INPUT_PULLUP);
        pinMode(page1ButtonState[i][1], INPUT_PULLUP);

    }

    // shift button ISR attaching
    attachInterrupt(digitalPinToInterrupt(shiftPin), shiftButtonISR, CHANGE);
    attachInterrupt(digitalPinToInterrupt(shiftEncoderState[0][0]), shiftEncoderISR, CHANGE);
    attachInterrupt(digitalPinToInterrupt(shiftEncoderState[0][1]), shiftEncoderISR, CHANGE);

    // rotary ISR attaching
    attachInterrupt(digitalPinToInterrupt(page1EncoderState[0][0]), pageEncoder1ISR, CHANGE);
    attachInterrupt(digitalPinToInterrupt(page1EncoderState[0][1]), pageEncoder1ISR, CHANGE);

    attachInterrupt(digitalPinToInterrupt(page1EncoderState[1][0]), pageEncoder2ISR, CHANGE);
    attachInterrupt(digitalPinToInterrupt(page1EncoderState[1][1]), pageEncoder2ISR, CHANGE);

    attachInterrupt(digitalPinToInterrupt(page1EncoderState[2][0]), pageEncoder3ISR, CHANGE);
    attachInterrupt(digitalPinToInterrupt(page1EncoderState[2][1]), pageEncoder3ISR, CHANGE);

    attachInterrupt(digitalPinToInterrupt(page1EncoderState[3][0]), pageEncoder4ISR, CHANGE);
    attachInterrupt(digitalPinToInterrupt(page1EncoderState[3][1]), pageEncoder4ISR, CHANGE);

    // pbutton ISR attaching
    attachInterrupt(digitalPinToInterrupt(page1ButtonState[0][0]), pageButton1ISR, CHANGE);
    attachInterrupt(digitalPinToInterrupt(page1ButtonState[1][0]), pageButton2ISR, CHANGE);
    attachInterrupt(digitalPinToInterrupt(page1ButtonState[2][0]), pageButton3ISR, CHANGE);
    attachInterrupt(digitalPinToInterrupt(page1ButtonState[3][0]), pageButton4ISR, CHANGE);

    // boot light show
    ledBoot();

}

void loop() {
    // shift handler
    if (shift) {
        displayPageSelectLED(curPage);

        if (curMode != prevMode) {
            prevMode = curMode;
        }

    }
    else {
        displayButtonLED(curPage);
        displayRotaryLED(curPage);
    }

    // check for midi message
    if (Serial.peek() == 0xB0) {
        parseMidiData(Serial);
    }

}
