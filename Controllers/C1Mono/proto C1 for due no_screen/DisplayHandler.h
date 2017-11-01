// Jordan Guzak
// fill in header

#ifndef DISPLAY_HANDLER_H
#define DISPLAY_HANDLER_H

#include "HardwareIO.h"


const static int numModes = 3;
volatile static int curMode = 1, prevMode = 1;
const static String modeName[numModes] = { "Performance", "View Midi Out", "View Midi In" };

// ***************************************
// led display functions

void ledClear() {
    for (int i = 0; i < pageButtonEncoders; i++) {
        digitalWrite(buttonLEDPins[i], LOW);
        digitalWrite(rotaryLEDPins[i], LOW);
    }
}

/* flicker selected page leds
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

void ledBoot() {
    for (int p = 0; p < pages; p++) {
        pinMode(buttonLEDPins[p], OUTPUT);
        pinMode(rotaryLEDPins[p], OUTPUT);

        for (int v = 0; v < 127; v++) {
            if (v != 126) {
                analogWrite(rotaryLEDPins[p], map(v, 0, 127, 0, 255));
                analogWrite(buttonLEDPins[p], map(v, 0, 127, 0, 255));
            }
            else {
                digitalWrite(rotaryLEDPins[p], LOW);
                digitalWrite(buttonLEDPins[p], LOW);
            }
            delay(2);

        }
        delay(100);
    }

    ledClear();
}

/** led output for page selection
*  @param p the current page
*/
void displayPageSelectLED(int p) {
    switch (p)
    {
    case 1:
        ledFlash(buttonLEDPins[0], rotaryLEDPins[0]);
        analogWrite(buttonLEDPins[1], 0);
        analogWrite(buttonLEDPins[2], 0);
        analogWrite(buttonLEDPins[3], 0);

        analogWrite(rotaryLEDPins[1], 0);
        analogWrite(rotaryLEDPins[2], 0);
        analogWrite(rotaryLEDPins[3], 0);

        break;
    case 2:
        ledFlash(buttonLEDPins[1], rotaryLEDPins[1]);

        analogWrite(buttonLEDPins[0], 0);
        analogWrite(buttonLEDPins[2], 0);
        analogWrite(buttonLEDPins[3], 0);

        analogWrite(rotaryLEDPins[0], 0);
        analogWrite(rotaryLEDPins[2], 0);
        analogWrite(rotaryLEDPins[3], 0);

        break;
    case 3:
        ledFlash(buttonLEDPins[2], rotaryLEDPins[2]);

        analogWrite(buttonLEDPins[0], 0);
        analogWrite(buttonLEDPins[1], 0);
        analogWrite(buttonLEDPins[3], 0);

        analogWrite(rotaryLEDPins[0], 0);
        analogWrite(rotaryLEDPins[1], 0);
        analogWrite(rotaryLEDPins[3], 0);

        break;
    case 4:
        ledFlash(buttonLEDPins[3], rotaryLEDPins[3]);

        analogWrite(buttonLEDPins[0], 0);
        analogWrite(buttonLEDPins[1], 0);
        analogWrite(buttonLEDPins[2], 0);

        analogWrite(rotaryLEDPins[0], 0);
        analogWrite(rotaryLEDPins[1], 0);
        analogWrite(rotaryLEDPins[2], 0);

        break;
    default:
        ledClear();
        break;
    }
}

/** led output for button states
*  @param p the current page to display
*/
void displayButtonLED(int p) {
    switch (p)
    {
    case 1:
        for (int i = 0; i < pageButtonEncoders; i++) {
            if (page1ButtonState[i][2] == 1) {
                analogWrite(buttonLEDPins[i], 255);
            }
            else {
                analogWrite(buttonLEDPins[i], 0);
            }
        }
        break;
    case 2:
        for (int i = 0; i < pageButtonEncoders; i++) {
            if (page2ButtonState[i][2] == 1) {
                analogWrite(buttonLEDPins[i], 0);
            }
            else {
                analogWrite(buttonLEDPins[i], 0);
            }
        }
        break;
    case 3:
        for (int i = 0; i < pageButtonEncoders; i++) {
            if (page3ButtonState[i][2] == 1) {
                analogWrite(buttonLEDPins[i], 255);
            }
            else {
                analogWrite(buttonLEDPins[i], 0);
            }
        }
        break;
    case 4:
        for (int i = 0; i < pageButtonEncoders; i++) {
            if (page4ButtonState[i][2] == 1) {
                analogWrite(buttonLEDPins[i], 255);
            }
            else {
                analogWrite(buttonLEDPins[i], 0);
            }
        }
        break;
    default:
        break;
    }
}

// led output for rotary state
void displayRotaryLED(int p) {
    switch (p)
    {
    case 1:
        for (int i = 0; i < pageButtonEncoders; i++) {
            analogWrite(rotaryLEDPins[i], map(page1EncoderState[i][7], 0, 127, 0, 255));
        }
        break;
    case 2:
        for (int i = 0; i < pageButtonEncoders; i++) {
            analogWrite(rotaryLEDPins[i], map(page2EncoderState[i][7], 0, 127, 0, 255));
        }
        break;
    case 3:
        for (int i = 0; i < pageButtonEncoders; i++) {
            analogWrite(rotaryLEDPins[i], map(page3EncoderState[i][7], 0, 127, 0, 255));
        }
        break;
    case 4:
        for (int i = 0; i < pageButtonEncoders; i++) {
            analogWrite(rotaryLEDPins[i], map(page4EncoderState[i][7], 0, 127, 0, 255));
        }
        break;
    default:
        break;
    }
}

#endif // !DISPLAY_HANDLER_H
