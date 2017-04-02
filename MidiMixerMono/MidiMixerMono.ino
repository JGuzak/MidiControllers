/*
 Name:		MidiMixerMono.ino
 Created:	4/1/2017 7:18:03 PM
 Author:	Jordan Guzak
*/

#include "RotaryHandler.h"
#include "ButtonHandler.h"

// **************************************
volatile static int *curData;

int pageButtonEncoders = 4;
volatile static int curPageEncoder;
volatile static int pageEncoderState[][ROTARY_VALUE_ARRAY_SIZE] = {
    { 46, 47, 0, 0, 0, 1, 10, 64 },
    { 48, 49, 0, 0, 0, 1, 11, 64 },
    { 50, 51, 0, 0, 0, 1, 12, 64 },
    { 52, 53, 0, 0, 0, 1, 13, 64 }
};

volatile static int curPageButton;
volatile static int pageButtonState[][BUTTON_ARRAY_SIZE] = {
    { 42, 0, 1, 1, 14 },
    { 43, 0, 1, 1, 15 },
    { 44, 0, 1, 1, 16 },
    { 45, 0, 1, 1, 17 }
};

/*
int channel1StaticEncoders = 2;
volatile static int curC1StaticEncoder;
volatile static int channel1StaticEncoderState[][ROTARY_VALUE_ARRAY_SIZE] = {
    {},
    {}
};

int channel1StaticButtons = 4;
volatile static int curC1StaticButton;
volatile static int channel1StaticButton[][BUTTON_ARRAY_SIZE] = {
    {},
    {},
    {},
    {}
};

*/


void setup() {
    Serial.begin(56000);

    for (int i = 0; i < pageButtonEncoders; i++) {
        pinMode(pageEncoderState[i][0], INPUT_PULLUP);
        pinMode(pageEncoderState[i][1], INPUT_PULLUP);
        pageEncoderState[i][2] = rotaryGetState(pageEncoderState[i][0], 
                                                pageEncoderState[i][1]);

        pinMode(pageButtonState[i][0], INPUT_PULLUP);
        pinMode(pageButtonState[i][1], INPUT_PULLUP);
        pageButtonState[i][2] = rotaryGetState(pageButtonState[i][0],
                                               pageButtonState[i][1]);
    }

    // page encoders ISR attaching
    attachInterrupt(digitalPinToInterrupt(pageEncoderState[0][0]), pageEncoder1ISR, CHANGE);
    attachInterrupt(digitalPinToInterrupt(pageEncoderState[0][1]), pageEncoder1ISR, CHANGE);

    attachInterrupt(digitalPinToInterrupt(pageEncoderState[1][0]), pageEncoder2ISR, CHANGE);
    attachInterrupt(digitalPinToInterrupt(pageEncoderState[1][1]), pageEncoder2ISR, CHANGE);

    attachInterrupt(digitalPinToInterrupt(pageEncoderState[2][0]), pageEncoder3ISR, CHANGE);
    attachInterrupt(digitalPinToInterrupt(pageEncoderState[2][1]), pageEncoder3ISR, CHANGE);

    attachInterrupt(digitalPinToInterrupt(pageEncoderState[3][0]), pageEncoder4ISR, CHANGE);
    attachInterrupt(digitalPinToInterrupt(pageEncoderState[3][1]), pageEncoder4ISR, CHANGE);

    // page button ISR attaching
    attachInterrupt(digitalPinToInterrupt(pageButtonState[0][0]), pageButton1ISR, CHANGE);
    attachInterrupt(digitalPinToInterrupt(pageButtonState[1][0]), pageButton2ISR, CHANGE);
    attachInterrupt(digitalPinToInterrupt(pageButtonState[2][0]), pageButton3ISR, CHANGE);
    attachInterrupt(digitalPinToInterrupt(pageButtonState[3][0]), pageButton4ISR, CHANGE);

}


void loop() {}

// page encoder ISR
void pageEncoder1ISR() {
    curPageEncoder = 0;
    curData = rotaryValueUpdateState(pageEncoderState[curPageEncoder]);

    for (int i = 0; i < ROTARY_VALUE_ARRAY_SIZE; i++) {
        pageEncoderState[curPageEncoder][i] = curData[i];
    }
}

void pageEncoder2ISR() {
    curPageEncoder = 1;
    curData = rotaryValueUpdateState(pageEncoderState[curPageEncoder]);

    for (int i = 0; i < ROTARY_VALUE_ARRAY_SIZE; i++) {
        pageEncoderState[curPageEncoder][i] = curData[i];
    }
}

void pageEncoder3ISR() {
    curPageEncoder = 2;
    curData = rotaryValueUpdateState(pageEncoderState[curPageEncoder]);

    for (int i = 0; i < ROTARY_VALUE_ARRAY_SIZE; i++) {
        pageEncoderState[curPageEncoder][i] = curData[i];
    }
}

void pageEncoder4ISR() {
    curPageEncoder = 3;
    curData = rotaryValueUpdateState(pageEncoderState[curPageEncoder]);

    for (int i = 0; i < ROTARY_VALUE_ARRAY_SIZE; i++) {
        pageEncoderState[curPageEncoder][i] = curData[i];
    }
}

// page button ISR
void pageButton1ISR() {
    curPageButton = 0;
    curData = buttonUpdateState(pageButtonState[curPageButton]);

    for (int i = 0; i < BUTTON_ARRAY_SIZE; i++) {
        pageButtonState[curPageButton][i] = curData[i];
    }
}

void pageButton2ISR() {
    curPageButton = 1;
    curData = buttonUpdateState(pageButtonState[curPageButton]);

    for (int i = 0; i < BUTTON_ARRAY_SIZE; i++) {
        pageButtonState[curPageButton][i] = curData[i];
    }
}

void pageButton3ISR() {
    curPageButton = 2;
    curData = buttonUpdateState(pageButtonState[curPageButton]);

    for (int i = 0; i < BUTTON_ARRAY_SIZE; i++) {
        pageButtonState[curPageButton][i] = curData[i];
    }
}

void pageButton4ISR() {
    curPageButton = 3;
    curData = buttonUpdateState(pageButtonState[curPageButton]);

    for (int i = 0; i < BUTTON_ARRAY_SIZE; i++) {
        pageButtonState[curPageButton][i] = curData[i];
    }
}