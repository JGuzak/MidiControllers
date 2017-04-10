// Jordan Guzak

// ***************************************
// input hardware handling

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
    displayPageRotaryCCValue(curPage);
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
