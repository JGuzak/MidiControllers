// Jordan Guzak

#ifndef ISR_ROUTINES_H
#define ISR_ROUTINES_H

// ***************************************
// shift encoder
void shiftEncoderISR() {
    if (shift) {
        updateShiftRotaryState(1);
    }
    else {
        updateShiftRotaryState(0);
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
    if (digitalRead(shiftPin) == HIGH) {
        shift = false;
    }
    else {
        shift = true;
    }
    ledClear();
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
