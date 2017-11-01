

#ifndef ISR_ROUTINES_H
#define ISR_ROUTINES_H

#include "HardwareIO.h"
#include "HardwareHandler.h"


// Jordan Guzak

// ***************************************
// shift encoder
void shiftEncoderISR() {
    if (shift) {
        curData = rotaryDeltaUpdateState(shiftEncoderState[1]);

        // check for delta value
        if (curData[7] == 65 && curPage < 4) {
            curPage += 1;
            curData[7] = 64;
        }
        else if (curData[7] == 63 && curPage > 1) {
            curPage -= 1;
            curData[7] = 64;
        }

        for (int i = 0; i < ROTARY_DELTA_ARRAY_SIZE; i++) {
            shiftEncoderState[1][i] = curData[i];
        }
    }
    else {
        curData = rotaryDeltaUpdateState(shiftEncoderState[0]);

        if (curData[7] == 65) {
            rotaryDeltaGenerateMidiMessage(curData);
            
        }
        else if (curData[7] == 63 ) {
            rotaryDeltaGenerateMidiMessage(curData);
            
        }

        


        curData[7] = 64;

        for (int i = 0; i < ROTARY_DELTA_ARRAY_SIZE; i++) {
            shiftEncoderState[0][i] = curData[i];
        }
    }
}

// page encoder ISRs
void pageEncoder1ISR() {
    curPageEncoder = 0;
    if (!shift) {
        updatePageRotaryState(curPageEncoder);
    }
}

void pageEncoder2ISR() {
    curPageEncoder = 1;
    if (!shift) {
        updatePageRotaryState(curPageEncoder);
    }
}

void pageEncoder3ISR() {
    curPageEncoder = 2;
    if (!shift) {
        updatePageRotaryState(curPageEncoder);
    }
}

void pageEncoder4ISR() {
    curPageEncoder = 3;
    if (!shift) {
        updatePageRotaryState(curPageEncoder);
    }
}

// shift button ISR
void shiftButtonISR() {

    ledClear();
    

    // update shift value
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

#endif // !ISR_ROUTINES_H
