/*
 Name:		RotaryISRExample.ino
 Created:	3/28/2017 2:45:13 PM
 Author:	Jordan Guzak
*/

#include "RotaryHandler.h"

// ******************************************
// To Do:
//  [ ] incorporate direction value into debouncing
//      system.
//  [ ] implement signal sends over midi

// for debugging rotary encoder generic functions
bool ISR_DEBUG = false; // all isr#() functions

// value based rotary group definition
static int rotaryValueData[][ROTARY_VALUE_ARRAY_SIZE] = {
    { 2, 3, 0, 0, 1, 1, 10, 0 }
};

// delta based rotary group definition
static int rotaryDeltaData[][ROTARY_DELTA_ARRAY_SIZE] = {
    { 2, 3, 0, 0, 1, 1, 10 }
};

static int *curData;
int numEncoders = 1;
int curValueRotary;
int curDeltaRotary;


void setup() {
	Serial.begin(56000);

    // for configuring value based rotary pins and interrupts
	
    for (int i = 0; i < numEncoders; i++) {
        pinMode(rotaryValueData[i][0], INPUT_PULLUP);
        pinMode(rotaryValueData[i][1], INPUT_PULLUP);
		rotaryValueData[i][2] = rotaryGetState(rotaryValueData[i][0], rotaryValueData[i][1]);
	}
	
	// attach isr# routines for each rotary
	attachInterrupt(digitalPinToInterrupt(rotaryValueData[0][0]), isr0, CHANGE);
    attachInterrupt(digitalPinToInterrupt(rotaryValueData[0][1]), isr0, CHANGE);
    
    
    // for configuring value based rotary pins and interrupts
    /*
    for (int i = 0; i < numEncoders; i++) {
        pinMode(rotaryDeltaData[i][0], INPUT_PULLUP);
        pinMode(rotaryDeltaData[i][1], INPUT_PULLUP);
        rotaryDeltaData[i][2] = rotaryGetState(rotaryDeltaData[i][0], rotaryDeltaData[i][1]);
    }
    */

    // attach isr# routines for each rotary
    //attachInterrupt(digitalPinToInterrupt(rotaryDeltaData[0][0]), isr1, CHANGE);
    //attachInterrupt(digitalPinToInterrupt(rotaryDeltaData[0][1]), isr1, CHANGE);

}

void loop() {}

// make one of these for every rotary encoder needed
void isr0() {
    curValueRotary = 0;
    // update state is a generic funtion that will handle both
    //  value based and delta based rotary data
    curData = rotaryValueUpdateState(rotaryValueData[curValueRotary]);

    // store values back into rotary data array
    for (int i = 0; i < ROTARY_VALUE_ARRAY_SIZE; i++) {
        rotaryValueData[curValueRotary][i] = curData[i];
    }

}

void isr1() {
    curDeltaRotary = 0;
    // update state is a generic funtion that will handle both
    //  value based and delta based rotary data
    curData = rotaryDeltaUpdateState(rotaryDeltaData[curDeltaRotary]);

    // store values back into rotary data array
    for (int i = 0; i < sizeof(curData) / sizeof(int); i++) {
        rotaryDeltaData[curValueRotary][i] = curData[i];
    }

}