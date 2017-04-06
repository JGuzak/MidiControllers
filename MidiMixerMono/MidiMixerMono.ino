/*
 Name:		MidiMixerMono.ino
 Created:	4/1/2017 7:18:03 PM
 Author:	Jordan Guzak
*/

#include "RotaryHandler.h"
#include "ButtonHandler.h"
#include "HardwareStates.h"

void setup() {
    Serial.begin(31250);

    pinMode(shiftPin, INPUT_PULLUP);
    pinMode(shiftEncoderState[0], INPUT_PULLUP);
    pinMode(shiftEncoderState[1], INPUT_PULLUP);

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
    attachInterrupt(digitalPinToInterrupt(shiftEncoderState[0]), shiftEncoderISR, CHANGE);
    attachInterrupt(digitalPinToInterrupt(shiftEncoderState[1]), shiftEncoderISR, CHANGE);

    // page encoders ISR attaching
    attachInterrupt(digitalPinToInterrupt(page1EncoderState[0][0]), pageEncoder1ISR, CHANGE);
    attachInterrupt(digitalPinToInterrupt(page1EncoderState[0][1]), pageEncoder1ISR, CHANGE);

    attachInterrupt(digitalPinToInterrupt(page1EncoderState[1][0]), pageEncoder2ISR, CHANGE);
    attachInterrupt(digitalPinToInterrupt(page1EncoderState[1][1]), pageEncoder2ISR, CHANGE);

    attachInterrupt(digitalPinToInterrupt(page1EncoderState[2][0]), pageEncoder3ISR, CHANGE);
    attachInterrupt(digitalPinToInterrupt(page1EncoderState[2][1]), pageEncoder3ISR, CHANGE);

    attachInterrupt(digitalPinToInterrupt(page1EncoderState[3][0]), pageEncoder4ISR, CHANGE);
    attachInterrupt(digitalPinToInterrupt(page1EncoderState[3][1]), pageEncoder4ISR, CHANGE);

    // page button ISR attaching
    attachInterrupt(digitalPinToInterrupt(page1ButtonState[0][0]), pageButton1ISR, CHANGE);
    attachInterrupt(digitalPinToInterrupt(page1ButtonState[1][0]), pageButton2ISR, CHANGE);
    attachInterrupt(digitalPinToInterrupt(page1ButtonState[2][0]), pageButton3ISR, CHANGE);
    attachInterrupt(digitalPinToInterrupt(page1ButtonState[3][0]), pageButton4ISR, CHANGE);

    // boot light show
    for (int p = 0; p < pages; p++) {
        pinMode(buttonLEDPins[p], OUTPUT);
        pinMode(rotaryLEDPins[p], OUTPUT);

        for (int v = 0; v < 127; v++) {
            analogWrite(rotaryLEDPins[p], map(v, 0, 126, 0, 255));
            analogWrite(buttonLEDPins[p], map(v, 0, 126, 0, 255));
            delay(2);
        }

        delay(100);
    }

    analogWrite(buttonLEDPins[0], 255);
    analogWrite(buttonLEDPins[1], 255);
    analogWrite(buttonLEDPins[2], 255);
    analogWrite(buttonLEDPins[3], 255);

    analogWrite(rotaryLEDPins[0], 255);
    analogWrite(rotaryLEDPins[1], 255);
    analogWrite(rotaryLEDPins[2], 255);
    analogWrite(rotaryLEDPins[3], 255);

}

/** flicker selected page leds
 *  @param pin1 first led to flicker
 *  @param pin2 second led to flicker
 */
void ledFlash(int pin1, int pin2) {
    int curTime = millis() % 511;

    if (curTime > 255) {
      curTime = map(curTime, 256, 511, 255, 0);
    }
    
    analogWrite(pin1, curTime);
    analogWrite(pin2, curTime);
    delay(2);
    
}

/** led output for page selection 
 *  @param p the current page
 */
void displayPageSelect(int p) {
    switch (p)
    {
    case 1:
        ledFlash(buttonLEDPins[0], rotaryLEDPins[0]);
        analogWrite(buttonLEDPins[1], HIGH);
        analogWrite(buttonLEDPins[2], HIGH);
        analogWrite(buttonLEDPins[3], HIGH);

        analogWrite(rotaryLEDPins[1], HIGH);
        analogWrite(rotaryLEDPins[2], HIGH);
        analogWrite(rotaryLEDPins[3], HIGH);

        break;
    case 2:
        ledFlash(buttonLEDPins[1], rotaryLEDPins[1]);

        analogWrite(buttonLEDPins[0], HIGH);
        analogWrite(buttonLEDPins[2], HIGH);
        analogWrite(buttonLEDPins[3], HIGH);

        analogWrite(rotaryLEDPins[0], HIGH);
        analogWrite(rotaryLEDPins[2], HIGH);
        analogWrite(rotaryLEDPins[3], HIGH);

        break;
    case 3:
        ledFlash(buttonLEDPins[2], rotaryLEDPins[2]);

        analogWrite(buttonLEDPins[0], HIGH);
        analogWrite(buttonLEDPins[1], HIGH);
        analogWrite(buttonLEDPins[3], HIGH);

        analogWrite(rotaryLEDPins[0], HIGH);
        analogWrite(rotaryLEDPins[1], HIGH);
        analogWrite(rotaryLEDPins[3], HIGH);

        break;
    case 4:
        ledFlash(buttonLEDPins[3], rotaryLEDPins[3]);

        analogWrite(buttonLEDPins[0], HIGH);
        analogWrite(buttonLEDPins[1], HIGH);
        analogWrite(buttonLEDPins[2], HIGH);

        analogWrite(rotaryLEDPins[0], HIGH);
        analogWrite(rotaryLEDPins[1], HIGH);
        analogWrite(rotaryLEDPins[2], HIGH);

        break;
    default:
        analogWrite(buttonLEDPins[0], HIGH);
        analogWrite(buttonLEDPins[1], HIGH);
        analogWrite(buttonLEDPins[2], HIGH);
        analogWrite(buttonLEDPins[3], HIGH);
        break;
    }
}

/** led output for button states 
 *  @param p the current page to display
 */
void displayButtonLEDs(int p) {
    switch (p)
    {
    case 1:
        for (int i = 0; i < pageButtonEncoders; i++) {
            if (page1ButtonState[i][2] == 1) {
                analogWrite(buttonLEDPins[i], LOW);
            }
            else {
                analogWrite(buttonLEDPins[i], HIGH);
            }
        }
        break;
    case 2:
        for (int i = 0; i < pageButtonEncoders; i++) {
            if (page2ButtonState[i][2] == 1) {
                analogWrite(buttonLEDPins[i], LOW);
            }
            else {
                analogWrite(buttonLEDPins[i], HIGH);
            }
        }
        break;
    case 3:
        for (int i = 0; i < pageButtonEncoders; i++) {
            if (page3ButtonState[i][2] == 1) {
                analogWrite(buttonLEDPins[i], LOW);
            }
            else {
                analogWrite(buttonLEDPins[i], HIGH);
            }
        }
        break;
    case 4:
        for (int i = 0; i < pageButtonEncoders; i++) {
            if (page4ButtonState[i][2] == 1) {
                analogWrite(buttonLEDPins[i], LOW);
            }
            else {
                analogWrite(buttonLEDPins[i], HIGH);
            }
        }
        break;
    default:
        break;
    }
}

// led output for rotary state
void displayRotaryLEDs(int p) {
    switch (p)
    {
    case 1:
        for (int i = 0; i < pageButtonEncoders; i++) {
            analogWrite(rotaryLEDPins[i], map(page1EncoderState[i][7], 0, 127, 255, 0));
        }
        break;
    case 2:
        for (int i = 0; i < pageButtonEncoders; i++) {
            analogWrite(rotaryLEDPins[i], map(page2EncoderState[i][7], 0, 127, 255, 0));
        }
        break;
    case 3:
        for (int i = 0; i < pageButtonEncoders; i++) {
            analogWrite(rotaryLEDPins[i], map(page3EncoderState[i][7], 0, 127, 255, 0));
        }
        break;
    case 4:
        for (int i = 0; i < pageButtonEncoders; i++) {
            analogWrite(rotaryLEDPins[i], map(page4EncoderState[i][7], 0, 127, 255, 0));
        }
        break;
    default:
        break;
    }

    

}

// led outputs are controlled in the loop
void loop() {
    // page led handling
    if (shift) {
        displayPageSelect(curPage);
    }
    else {
       displayButtonLEDs(curPage);
       displayRotaryLEDs(curPage);
    }

}

// page handling for rotary states
void updatePageRotaryState(int page) {
    switch (curPage)
    {
    case 1:
        curData = rotaryValueUpdateState(page1EncoderState[curPageEncoder]);
        for (int i = 0; i < ROTARY_VALUE_ARRAY_SIZE; i++) {
            page1EncoderState[curPageEncoder][i] = curData[i];
        }
        break;
    case 2:
        curData = rotaryValueUpdateState(page2EncoderState[curPageEncoder]);
        for (int i = 0; i < ROTARY_VALUE_ARRAY_SIZE; i++) {
            page2EncoderState[curPageEncoder][i] = curData[i];
        }
        break;
    case 3:
        curData = rotaryValueUpdateState(page3EncoderState[curPageEncoder]);
        for (int i = 0; i < ROTARY_VALUE_ARRAY_SIZE; i++) {
            page3EncoderState[curPageEncoder][i] = curData[i];
        }
        break;
    case 4:
        curData = rotaryValueUpdateState(page4EncoderState[curPageEncoder]);
        for (int i = 0; i < ROTARY_VALUE_ARRAY_SIZE; i++) {
            page4EncoderState[curPageEncoder][i] = curData[i];
        }
        break;
    default:
        curData = rotaryValueUpdateState(page1EncoderState[curPageEncoder]);
        for (int i = 0; i < ROTARY_VALUE_ARRAY_SIZE; i++) {
            page1EncoderState[curPageEncoder][i] = curData[i];
        }
    }

}

// page handling for button states
void updatePageButtonState(int page) {
    switch (page)
    {
    case 1:
        curData = buttonUpdateState(page1ButtonState[curPageButton]);
        for (int i = 0; i < BUTTON_ARRAY_SIZE; i++) {
            page1ButtonState[curPageButton][i] = curData[i];
        }
        break;
    case 2:
        curData = buttonUpdateState(page2ButtonState[curPageButton]);
        for (int i = 0; i < BUTTON_ARRAY_SIZE; i++) {
            page2ButtonState[curPageButton][i] = curData[i];
        }
        break;
    case 3:
        curData = buttonUpdateState(page3ButtonState[curPageButton]);
        for (int i = 0; i < BUTTON_ARRAY_SIZE; i++) {
            page3ButtonState[curPageButton][i] = curData[i];
        }
        break;
    case 4:
        curData = buttonUpdateState(page4ButtonState[curPageButton]);
        for (int i = 0; i < BUTTON_ARRAY_SIZE; i++) {
            page4ButtonState[curPageButton][i] = curData[i];
        }
        break;
    default:
        break;
    }
}

void shiftEncoderISR() {
    if (shift) {

    }
    else {
        curData = rotaryDeltaUpdateState(shiftEncoderState);

        for (int i = 0; i < ROTARY_DELTA_ARRAY_SIZE; i++) {
            shiftEncoderState[i] = curData[i];
        }
    }
}

// page encoder ISRs
void pageEncoder1ISR() {
    curPageEncoder = 0;
    updatePageRotaryState(curPageEncoder);
}

void pageEncoder2ISR() {
    curPageEncoder = 1;
    updatePageRotaryState(curPageEncoder);
}

void pageEncoder3ISR() {
    curPageEncoder = 2;
    updatePageRotaryState(curPageEncoder);
}

void pageEncoder4ISR() {
    curPageEncoder = 3;
    updatePageRotaryState(curPageEncoder);
}

// shift button ISR
void shiftButtonISR() {
    if (digitalRead(shiftPin) == HIGH) {
        shift = false;
    }
    else {
        shift = true;
    }
}

// page button ISRs
void pageButton1ISR() {
    curPageButton = 0;
    if (shift) {
        curPage = 1;
    }
    else {
        updatePageButtonState(curPage);
    }
}

void pageButton2ISR() {
    curPageButton = 1;
    if (shift) {
        curPage = 2;
    }
    else {
        updatePageButtonState(curPage);
    }
}

void pageButton3ISR() {
    curPageButton = 2;
    if (shift) {
        curPage = 3;
    }
    else {
        updatePageButtonState(curPage);
    }
}

void pageButton4ISR() {
    curPageButton = 3;
    if (shift) {
        curPage = 4;
    }
    else {
        updatePageButtonState(curPage);
    }
}
