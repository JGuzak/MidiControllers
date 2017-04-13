// Jordan Guzak

#ifndef HARDWARE_HANDLER_H
#define HARDWARE_HANDLER_H

// ***************************************
// input hardware handling

int rotaryValueCycleCompletion(volatile int data[]) {
    int val = 0;

    // check for send state and update value accordingly
    if (data[2] == 3) {
        if (data[4] == 1) {
            if (ROTARY_VALUE_MIN <= data[7] && data[7] < ROTARY_VALUE_MAX) {
                val = 1;
            }
        }
        else {
            if (ROTARY_VALUE_MIN < data[7] && data[7] <= ROTARY_VALUE_MAX) {
                val = -1;
            }
        }

    }

    return val;
}

int rotaryDeltaCycleCompletion(volatile int data[]) {
    int val = 0;

    // check for send state and update value accordingly
    if (data[2] == 3) {
        if (data[4] == 1) {
            val = 1;
        }
        else {
            val = -1;
        }
    }

    return val;
}

// page handling for rotary states
void updatePageRotaryState(int page) {
    switch (curPage)
    {
    case 1:
        curData = rotaryValueUpdateState(page1EncoderState[curPageEncoder]);

        // update stored value if cycle completes
        if (rotaryValueCycleCompletion(curData) == 1) {
            curData[2] = 0;
            curData[7] += 1;
            displayPageRotaryCCValueLCD(curPage);
            rotaryValGenerateMidiMessage(curData);
        }
        else if (rotaryValueCycleCompletion(curData) == -1) {
            curData[2] = 0;
            curData[7] -= 1;
            displayPageRotaryCCValueLCD(curPage);
            rotaryValGenerateMidiMessage(curData);
        }

        // update stored data
        for (int i = 0; i < ROTARY_VALUE_ARRAY_SIZE; i++) {
            page1EncoderState[curPageEncoder][i] = curData[i];
        }
        break;
    case 2:
        curData = rotaryValueUpdateState(page2EncoderState[curPageEncoder]);

        // update stored value if cycle completes
        if (rotaryValueCycleCompletion(curData) == 1) {
            curData[2] = 0;
            curData[7] += 1;
            displayPageRotaryCCValueLCD(curPage);
            rotaryValGenerateMidiMessage(curData);
        }
        else if (rotaryValueCycleCompletion(curData) == -1) {
            curData[2] = 0;
            curData[7] -= 1;
            displayPageRotaryCCValueLCD(curPage);
            rotaryValGenerateMidiMessage(curData);
        }

        // update stored data
        for (int i = 0; i < ROTARY_VALUE_ARRAY_SIZE; i++) {
            page2EncoderState[curPageEncoder][i] = curData[i];
        }
        break;
    case 3:
        curData = rotaryValueUpdateState(page3EncoderState[curPageEncoder]);

        // update stored value if cycle completes
        if (rotaryValueCycleCompletion(curData) == 1) {
            curData[2] = 0;
            curData[7] += 1;
            displayPageRotaryCCValueLCD(curPage);
            rotaryValGenerateMidiMessage(curData);
        }
        else if (rotaryValueCycleCompletion(curData) == -1) {
            curData[2] = 0;
            curData[7] -= 1;
            displayPageRotaryCCValueLCD(curPage);
            rotaryValGenerateMidiMessage(curData);
        }

        // update stored data
        for (int i = 0; i < ROTARY_VALUE_ARRAY_SIZE; i++) {
            page3EncoderState[curPageEncoder][i] = curData[i];
        }
        break;
    case 4:
        curData = rotaryValueUpdateState(page4EncoderState[curPageEncoder]);

        // update stored value if cycle completes
        if (rotaryValueCycleCompletion(curData) == 1) {
            curData[2] = 0;
            curData[7] += 1;
            displayPageRotaryCCValueLCD(curPage);
            rotaryValGenerateMidiMessage(curData);
        }
        else if (rotaryValueCycleCompletion(curData) == -1) {
            curData[2] = 0;
            curData[7] -= 1;
            displayPageRotaryCCValueLCD(curPage);
            rotaryValGenerateMidiMessage(curData);
        }

        // update stored data
        for (int i = 0; i < ROTARY_VALUE_ARRAY_SIZE; i++) {
            page4EncoderState[curPageEncoder][i] = curData[i];
        }
        break;
    default:
        break;
    }
}

// called in shift isr function
void updateShiftRotaryState(int curShiftEncoder) {
    curData = rotaryDeltaUpdateState(shiftEncoderState[curShiftEncoder]);

    // update stored value if cycle completes
    if (rotaryDeltaCycleCompletion(curData) == 1) {
        curData[2] = 0;
        curData[7] = 1;

        if (curShiftEncoder == 0) {
            rotaryDeltaGenerateMidiMessage(curData);
        }
        else {
            if (curMode < numModes-1) {
                curMode += curData[7];
            }

            displayCurModeLCD(curMode);

        }
    }
    else if (rotaryDeltaCycleCompletion(curData) == -1) {
        curData[2] = 0;
        curData[7] = -1;

        if (curShiftEncoder == 0) {
            rotaryDeltaGenerateMidiMessage(curData);
        }
        else {
            if (curMode > 0) {
                curMode += curData[7];
            }
        }
    }
    else {
        curData[7] = 0;
    }

    // update stored data
    for (int i = 0; i < ROTARY_DELTA_ARRAY_SIZE; i++) {
        shiftEncoderState[curShiftEncoder][i] = curData[i];
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

    displayPageButtonCCValueLCD(curPage);
}

#endif // !HARDWARE_HANDLER_H