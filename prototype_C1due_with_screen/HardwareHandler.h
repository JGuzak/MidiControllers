// Jordan Guzak

// ***************************************
// input hardware handling

#ifndef HARDWARE_HANDLER_H
#define HARDWARE_HANDLER_H

/*
* R: page handling for rotary states
* M: updates rotary state of paged encoder
*/
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
    
    if (curMode == 2) {
        displayPageRotaryLCD(curPage);
    }
}

/* 
* R: page handling for button states
* M: updates state of paged button
*/
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

    if (curMode == 2) {
        displayPageButtonLCD(curPage);
    }

}

/*
* R: cc and value to update
* M: value of data for given cc
*/
void updateMachineState(int cc, int val) {

    // check paged values
    for (int i = 0; i < pages; i++) {
        
        if (10 <= cc <= 25) {
            // check encoder cc values
            if (page1EncoderState[i][6] == cc) {
                page1EncoderState[i][7] = val;
                break;
            }

            if (page2EncoderState[i][6] == cc) {
                page2EncoderState[i][7] = val;
                break;
            }

            if (page3EncoderState[i][6] == cc) {
                page3EncoderState[i][7] = val;
                break;
            }

            if (page4EncoderState[i][6] == cc) {
                page4EncoderState[i][7] = val;
                break;
            }
        }
        else if (26 <= cc <= 41) {
            // check button cc values
            if (page1ButtonState[i][4] == cc) {
                page1ButtonState[i][2] = val;
                break;
            }

            if (page2ButtonState[i][4] == cc) {
                page2ButtonState[i][2] = val;
                break;
            }

            if (page3ButtonState[i][4] == cc) {
                page3ButtonState[i][2] = val;
                break;
            }

            if (page4ButtonState[i][4] == cc) {
                page4ButtonState[i][2] = val;
                break;
            }

        }

    }
}

/*
* R: port to read on
* M: calls machine state updates based on midi data
*/
void parseMidiData(UARTClass s) {
    byte cmdByte, channelByte, ccByte, valueByte;
    
    if (s.available() > 2) {
        cmdByte = s.read();
        ccByte = s.read();
        valueByte = s.read();
        channelByte = s.read();

        if (curMode == 3) {
            lcd.clear();
            lcd.setCursor(0, 0);
            lcd.print("Received midi:");
            lcd.setCursor(0, 1);
            lcd.print(ccByte);
            lcd.print(" ");
            lcd.print(valueByte);
            lcd.print(" ");
        }

        updateMachineState(ccByte, valueByte);
    }

}

#endif // !HARDWARE_HANDLER_H
