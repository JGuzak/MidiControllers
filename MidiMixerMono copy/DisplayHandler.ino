// Jordan Guzak

#ifndef DISPLAY_HANDLER_H
#define DISPLAY_HANDLER_H


// ***************************************
// led display functions

void ledClear() {
    for (int i = 0; i < pageButtonEncoders; i++) {
        digitalWrite(buttonLEDPins[i], HIGH);
        digitalWrite(rotaryLEDPins[i], HIGH);
    }
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
void displayPageSelectLED(int p) {
    switch (p)
    {
    case 1:
        ledFlash(buttonLEDPins[0], rotaryLEDPins[0]);
        analogWrite(buttonLEDPins[1], 255);
        analogWrite(buttonLEDPins[2], 255);
        analogWrite(buttonLEDPins[3], 255);

        analogWrite(rotaryLEDPins[1], 255);
        analogWrite(rotaryLEDPins[2], 255);
        analogWrite(rotaryLEDPins[3], 255);

        break;
    case 2:
        ledFlash(buttonLEDPins[1], rotaryLEDPins[1]);

        analogWrite(buttonLEDPins[0], 255);
        analogWrite(buttonLEDPins[2], 255);
        analogWrite(buttonLEDPins[3], 255);

        analogWrite(rotaryLEDPins[0], 255);
        analogWrite(rotaryLEDPins[2], 255);
        analogWrite(rotaryLEDPins[3], 255);

        break;
    case 3:
        ledFlash(buttonLEDPins[2], rotaryLEDPins[2]);

        analogWrite(buttonLEDPins[0], 255);
        analogWrite(buttonLEDPins[1], 255);
        analogWrite(buttonLEDPins[3], 255);

        analogWrite(rotaryLEDPins[0], 255);
        analogWrite(rotaryLEDPins[1], 255);
        analogWrite(rotaryLEDPins[3], 255);

        break;
    case 4:
        ledFlash(buttonLEDPins[3], rotaryLEDPins[3]);

        analogWrite(buttonLEDPins[0], 255);
        analogWrite(buttonLEDPins[1], 255);
        analogWrite(buttonLEDPins[2], 255);

        analogWrite(rotaryLEDPins[0], 255);
        analogWrite(rotaryLEDPins[1], 255);
        analogWrite(rotaryLEDPins[2], 255);

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
                analogWrite(buttonLEDPins[i], 0);
            }
            else {
                analogWrite(buttonLEDPins[i], 255);
            }
        }
        break;
    case 2:
        for (int i = 0; i < pageButtonEncoders; i++) {
            if (page2ButtonState[i][2] == 1) {
                analogWrite(buttonLEDPins[i], 0);
            }
            else {
                analogWrite(buttonLEDPins[i], 255);
            }
        }
        break;
    case 3:
        for (int i = 0; i < pageButtonEncoders; i++) {
            if (page3ButtonState[i][2] == 1) {
                analogWrite(buttonLEDPins[i], 0);
            }
            else {
                analogWrite(buttonLEDPins[i], 255);
            }
        }
        break;
    case 4:
        for (int i = 0; i < pageButtonEncoders; i++) {
            if (page4ButtonState[i][2] == 1) {
                analogWrite(buttonLEDPins[i], 0);
            }
            else {
                analogWrite(buttonLEDPins[i], 255);
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


// ***************************************
// lcd display functions

void displayPageRotaryCCValueLCD(int page) {
    if (curMode == 2) {
        //lcd.setCursor(0, 1);
        //lcd.print("                ");
        lcd.setCursor(0, 1);
        lcd.print("Rtry ");
        lcd.print(curPageEncoder);
        lcd.print(": ");

        switch (page)
        {
        case 1:
            curData = page1EncoderState[curPageEncoder];

            break;
        case 2:
            curData = page2EncoderState[curPageEncoder];
            break;
        case 3:
            curData = page3EncoderState[curPageEncoder];
            break;
        case 4:
            curData = page4EncoderState[curPageEncoder];
            break;

        default:
            break;
        }
        lcd.print(curData[5]);
        lcd.print(" ");
        lcd.print(curData[6]);
        lcd.print(" ");
        lcd.print(curData[7]);
    
    }
}

void displayPageButtonCCValueLCD(int page) {
    if (curMode == 2) {
        lcd.setCursor(0, 1);
        lcd.print("                ");
        lcd.setCursor(0,1);
        lcd.print("Btn ");
        lcd.print(curPageButton);
        lcd.print(": ");

        switch (page)
        {
        case 1:
            curData = page1ButtonState[curPageButton];
            break;
        case 2:
            curData = page2ButtonState[curPageButton];
            break;
        case 3:
            curData = page3ButtonState[curPageButton];
            break;
        case 4:
            curData = page4ButtonState[curPageButton];
            break;
        default:
            break;
        }



        lcd.print(curData[3]);
        lcd.print(" ");
        lcd.print(curData[4]);
        lcd.print(" ");
        lcd.print(curData[2]);
    }
}

void displayCurPageLCD(int page) {
    // clear top row
    lcd.setCursor(0,0);
    lcd.print("Page: ");
    lcd.print(page);
    lcd.print("         ");
}

void displayCurModeLCD(int m) {

    // clear screen contents
    //lcd.setCursor(0, 1);
    //lcd.print("                ");
    lcd.setCursor(0,1);

    lcd.print("Mode: ");
    lcd.print(modeName[m]);

}

#endif // !DISPLAY_HANDLER_H
