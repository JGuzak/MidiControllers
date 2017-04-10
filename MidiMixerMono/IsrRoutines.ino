// Jordan Guzak

// ***************************************
// shift encoder
void shiftEncoderISR() {
    if (shift) {
        curData = rotaryDeltaUpdateState(shiftEncoderState[1]);

        for (int i = 0; i < ROTARY_DELTA_ARRAY_SIZE; i++) {
            shiftEncoderState[1][i] = curData[i];
        }
    }
    else {
        curData = rotaryDeltaUpdateState(shiftEncoderState[0]);

        for (int i = 0; i < ROTARY_DELTA_ARRAY_SIZE; i++) {
            shiftEncoderState[0][i] = curData[i];
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