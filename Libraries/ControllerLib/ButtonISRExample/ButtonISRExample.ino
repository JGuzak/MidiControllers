/*
 Name:		ButtonISRExample.ino
 Created:	3/31/2017 11:11:38 AM
 Author:	Jordan Guzak
*/



#include "ButtonHandler.h"

// ******************************************
// To Do:
//  [ ] implement signal sends over midi

static int buttonData[][BUTTON_ARRAY_SIZE] = {
    {2, 1, 0, 1, 30 }
};
static int *curData;
int numButtons = 1;
int curButton;

void setup() {
    Serial.begin(56000);

    // set pin modes
    for (int i = 0; i < numButtons; i++) {
        pinMode(buttonData[i][0], INPUT_PULLUP);
        buttonData[i][2] = buttonGetState(buttonData[i][0]);
    }

    // attach interrupts
    attachInterrupt(digitalPinToInterrupt(buttonData[0][0]), isr0, CHANGE);

}

void loop() {}

// function runs every time a change is detected
void isr0() {
    curButton = 0;
    curData = buttonUpdateState(buttonData[curButton]);

    for (int i = 0; i < BUTTON_ARRAY_SIZE; i++) {
        buttonData[curButton][i] = curData[i];
    }
}
