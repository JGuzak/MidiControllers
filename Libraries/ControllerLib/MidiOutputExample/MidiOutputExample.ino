/*
 Name:		MidiOutputExample.ino
 Created:	3/31/2017 3:02:55 PM
 Author:	Jordan Guzak
*/



#include <MIDI.h>
#include "ButtonHandler.h"
#include "RotaryHandler.h"

static int button[BUTTON_ARRAY_SIZE] = {2, 0, 0, 1, 10};
static int *data;



void setup() {
    Serial.begin(56000);

    pinMode(button[0], INPUT_PULLUP);
    button[2] = buttonGetState(button[0]);

    attachInterrupt(digitalPinToInterrupt(button[0]), isr0, CHANGE);
    
    //MIDI.begin(MIDI_CHANNEL_OMNI);

}


void loop() {}

void isr0() {
    data = buttonUpdateState(button);

    for (int i = 0; i < BUTTON_ARRAY_SIZE; i++) {
        button[i] = data[i];
    }
    //MIDI.sendNoteOn(button[3], button[4], 0);
}