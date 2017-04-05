/*
 Name:		MidiMixerMono.ino
 Created:	4/1/2017 7:18:03 PM
 Author:	Jordan Guzak
*/

//#include <MIDI.h>

//MIDI_CREATE_DEFAULT_INSTANCE();

#include <MIDIUSB.h>

#include "RotaryHandler.h"
#include "ButtonHandler.h"

// **************************************
volatile static int *curData;
volatile static bool shift = false;
volatile static int shiftEncoderState[][ROTARY_DELTA_ARRAY_SIZE] = {
    {}
};

const static int shiftPin = 6;


const static int pages = 4;
static int curPage = 1;
const static int pageLEDPins[pages] = { 2, 3, 4, 5 };


const int pageButtonEncoders = 4;
volatile static int curPageEncoder;
volatile static int page1EncoderState[][ROTARY_VALUE_ARRAY_SIZE] = {
    { 46, 47, 0, 0, 0, 1, 10, 64 },
    { 48, 49, 0, 0, 0, 1, 11, 64 },
    { 50, 51, 0, 0, 0, 1, 12, 64 },
    { 52, 53, 0, 0, 0, 1, 13, 64 }
};
volatile static int page2EncoderState[][ROTARY_VALUE_ARRAY_SIZE] = {
    { 46, 47, 0, 0, 0, 1, 14, 64 },
    { 48, 49, 0, 0, 0, 1, 15, 64 },
    { 50, 51, 0, 0, 0, 1, 16, 64 },
    { 52, 53, 0, 0, 0, 1, 17, 64 }
};
volatile static int page3EncoderState[][ROTARY_VALUE_ARRAY_SIZE] = {
    { 46, 47, 0, 0, 0, 1, 18, 64 },
    { 48, 49, 0, 0, 0, 1, 19, 64 },
    { 50, 51, 0, 0, 0, 1, 20, 64 },
    { 52, 53, 0, 0, 0, 1, 21, 64 }
};
volatile static int page4EncoderState[][ROTARY_VALUE_ARRAY_SIZE] = {
    { 46, 47, 0, 0, 0, 1, 22, 64 },
    { 48, 49, 0, 0, 0, 1, 23, 64 },
    { 50, 51, 0, 0, 0, 1, 24, 64 },
    { 52, 53, 0, 0, 0, 1, 25, 64 }
};

volatile static int curPageButton;
volatile static int page1ButtonState[][BUTTON_ARRAY_SIZE] = {
    { 42, 0, 0, 1, 26 },
    { 43, 0, 0, 1, 27 },
    { 44, 0, 0, 1, 28 },
    { 45, 0, 0, 1, 29 }
};
volatile static int page2ButtonState[][BUTTON_ARRAY_SIZE] = {
    { 42, 0, 0, 1, 30 },
    { 43, 0, 0, 1, 31 },
    { 44, 0, 0, 1, 32 },
    { 45, 0, 0, 1, 33 }
};
volatile static int page3ButtonState[][BUTTON_ARRAY_SIZE] = {
    { 42, 0, 0, 1, 34 },
    { 43, 0, 0, 1, 35 },
    { 44, 0, 0, 1, 36 },
    { 45, 0, 0, 1, 37 }
};
volatile static int page4ButtonState[][BUTTON_ARRAY_SIZE] = {
    { 42, 0, 0, 1, 38 },
    { 43, 0, 0, 1, 39 },
    { 44, 0, 0, 1, 40 },
    { 45, 0, 0, 1, 41 }
};


void setup() {
    //Serial.begin(56000);

    //MIDI.begin(MIDI_CHANNEL_OMNI);

    Serial.begin(115200);

    pinMode(shiftPin, INPUT_PULLUP);

    for (int p = 0; p < pages; p++) {
        pinMode(pageLEDPins[p], OUTPUT);
        digitalWrite(pageLEDPins[p], LOW);
        delay(200);
    }

    digitalWrite(pageLEDPins[0], HIGH);
    digitalWrite(pageLEDPins[1], HIGH);
    digitalWrite(pageLEDPins[2], HIGH);
    digitalWrite(pageLEDPins[3], HIGH);

    for (int i = 0; i < pageButtonEncoders; i++) {
        pinMode(page1EncoderState[i][0], INPUT_PULLUP);
        pinMode(page1EncoderState[i][1], INPUT_PULLUP);

        pinMode(page1ButtonState[i][0], INPUT_PULLUP);
        pinMode(page1ButtonState[i][1], INPUT_PULLUP);

    }

    // shift button ISR attaching
    attachInterrupt(digitalPinToInterrupt(shiftPin), shiftISR, CHANGE);

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

}

void ledFlash(int pin) {
    if (millis() % 200 > 100) {
        digitalWrite(pin, LOW);
    }
    else {
        digitalWrite(pin, HIGH);
    }
}

// led outputs are controlled in the loop
void loop() {
    // page led handling
    if (shift) {
        switch (curPage)
            {
            case 1:
                ledFlash(pageLEDPins[0]);
                digitalWrite(pageLEDPins[1], HIGH);
                digitalWrite(pageLEDPins[2], HIGH);
                digitalWrite(pageLEDPins[3], HIGH);
                break;
            case 2:
                digitalWrite(pageLEDPins[0], HIGH);
                ledFlash(pageLEDPins[1]);
                digitalWrite(pageLEDPins[2], HIGH);
                digitalWrite(pageLEDPins[3], HIGH);
                break;
            case 3:
                digitalWrite(pageLEDPins[0], HIGH);
                digitalWrite(pageLEDPins[1], HIGH);
                ledFlash(pageLEDPins[2]);
                digitalWrite(pageLEDPins[3], HIGH);
                break;
            case 4:
                digitalWrite(pageLEDPins[0], HIGH);
                digitalWrite(pageLEDPins[1], HIGH);
                digitalWrite(pageLEDPins[2], HIGH);
                ledFlash(pageLEDPins[3]);
                break;
            default:
                digitalWrite(pageLEDPins[0], HIGH);
                digitalWrite(pageLEDPins[1], HIGH);
                digitalWrite(pageLEDPins[2], HIGH);
                digitalWrite(pageLEDPins[3], HIGH);
                break;
            }
    }
    else {
        switch (curPage)
        {
        case 1:
            for (int i = 0; i < pageButtonEncoders; i++) {
                if (page1ButtonState[i][2] == 1) {
                    digitalWrite(pageLEDPins[i], LOW);
                }
                else {
                    digitalWrite(pageLEDPins[i], HIGH);
                }
            }
            break;
        case 2:
            for (int i = 0; i < pageButtonEncoders; i++) {
                if (page2ButtonState[i][2] == 1) {
                    digitalWrite(pageLEDPins[i], LOW);
                }
                else {
                    digitalWrite(pageLEDPins[i], HIGH);
                }
            }
            break;
        case 3:
            for (int i = 0; i < pageButtonEncoders; i++) {
                if (page3ButtonState[i][2] == 1) {
                    digitalWrite(pageLEDPins[i], LOW);
                }
                else {
                    digitalWrite(pageLEDPins[i], HIGH);
                }
            }
            break;
        case 4:
            for (int i = 0; i < pageButtonEncoders; i++) {
                if (page4ButtonState[i][2] == 1) {
                    digitalWrite(pageLEDPins[i], LOW);
                }
                else {
                    digitalWrite(pageLEDPins[i], HIGH);
                }
            }
            break;
        default:
            break;
        }
    }

}

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
        curData = buttonUpdateState(page1ButtonState[curPageButton]);
        for (int i = 0; i < BUTTON_ARRAY_SIZE; i++) {
            page1ButtonState[curPageButton][i] = curData[i];
        }
        break;
    }
}

// shift button ISR
void shiftISR() {
    if (digitalRead(shiftPin) == HIGH) {
        shift = false;
    }
    else {
        shift = true;
    }
}

// page encoder ISR
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

// page button ISR
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
