/*
Name:		C1Mono.ino
Created:	4/8/2017 1:54:56 PM
Author:	Jordan Guzak
*/

#include <Wire.h>
#include <SparkFunSX1509.h>
#include <Encoder.h>

// constants:
const bool SERIAL_OUTPUT = true;
const bool MIDI_OUTPUT = false;

const int MAX_ENCODER_VAL = 512;
const int NUM_BANKS = 4;
const int NUM_ENCODERS = 5;

const int rotaryAPin[NUM_ENCODERS] = { 2, 4, 6, 8, 10 };
const int rotaryBPin[NUM_ENCODERS] = { 3, 5, 7, 9, 11 };
const byte buttonPin[NUM_ENCODERS] = { 0, 1, 2, 3, 4 };
const byte rotaryLEDPin[NUM_ENCODERS-1] = { 5, 6, 7, 8 };
const byte buttonLEDPin[NUM_ENCODERS-1] = { 9, 10, 11, 12 };

const byte SX1509_ADDRESS = 0x3E;
SX1509 io;

Encoder rotaryEncoder[NUM_ENCODERS] = {
    { rotaryAPin[0], rotaryBPin[0] },
    { rotaryAPin[1], rotaryBPin[1] },
    { rotaryAPin[2], rotaryBPin[2] },
    { rotaryAPin[3], rotaryBPin[3] },
    { rotaryAPin[4], rotaryBPin[4] }
};
const int midiRotaryCC[NUM_ENCODERS][NUM_BANKS] = {
    { 10, 11, 12, 13, 26 },
    { 14, 15, 16, 17, 26 },
    { 18, 19, 20, 21, 26 },
    { 22, 23, 24, 25, 26 }
};
const int midiButtonCC[NUM_ENCODERS-1][NUM_BANKS] = {
    { 27, 28, 29, 30 },
    { 31, 32, 33, 34 },
    { 35, 36, 37, 38 },
    { 39, 40, 41, 42 }
};
volatile int rotaryValue[NUM_ENCODERS-1][NUM_BANKS] = {
    { 0, 0, 0, 0 },
    { 0, 0, 0, 0 },
    { 0, 0, 0, 0 },
    { 0, 0, 0, 0 }
};
volatile int buttonValue[NUM_ENCODERS-1][NUM_BANKS] = {
    { 0, 0, 0, 0 },
    { 0, 0, 0, 0 },
    { 0, 0, 0, 0 },
    { 0, 0, 0, 0 }
};
volatile bool shiftMode = false;
volatile int curBank = 0, shiftRotaryValue = 0;

void setup() {
    Serial.begin(56000);
    // Call io.begin(<address>) to initialize the SX1509. If it 
    // successfully communicates, it'll return 1.
    if (!io.begin(SX1509_ADDRESS)) 
    {
        while (1) ; // If we fail to communicate, loop forever.
    }
    // initalize led and button pins on i/o expander board
    for (int i = 0; i < NUM_ENCODERS - 1; i++) {
        io.pinMode(buttonPin[i], DIGITAL_INPUT);
        if ( i <= NUM_ENCODERS-1) {
            io.pinMode(rotaryLEDPin[i], ANALOG_OUTPUT);
            io.pinMode(buttonLEDPin[i], ANALOG_OUTPUT);
        }
    }
    ledBoot();
}

void loop() {
    rotaryHandler();
    buttonHandler();
    ledDisplay();
}

// TODO: verify encoders work
// TODO: verify banking works
// TODO: verify shift encoder works properly    `
void rotaryHandler() {
    // check for shift mode
    if (shiftMode) {
    }
    else {
        // sets the rotary encoders to the proper bank values and cycles
        // through all 4 rotaries on the selected bank and check for changes.
        for (int i = 0; i < (NUM_ENCODERS-1)) {
            rotaryEncoder[i].write(rotaryValue[i][curBank]);
        }
        for (int i = 0; i < (NUM_ENCODERS); i++) {
            long newRotaryValue;
            newRotaryValue = rotaryEncoder[i].read();
            // check for rotary changes, do nothing otherwise
            // for banked rotaries
            if ((newRotaryValue >= (rotaryValue[i][curBank] + 4)) || (newRotaryValue <= (rotaryValue[i][curBank] - 4))) {
                // for banked rotary encoders:
                // check for min/max value, update value otherwise
                // handle edge cases where rotary would excede maximum and minimum values
                if (i < (NUM_ENCODERS-2)) {
                    if (newRotaryValue < 0) {
                        rotaryEncoder[i][curBank].write(0);
                    }
                    else if (newRotaryValue > MAX_ENCODER_VAL) {
                        rotaryEncoder[i][curBank].write(MAX_ENCODER_VAL);
                    }
                    else {
                        rotaryEncoder[i][curBank].write(newRotaryValue);
                        rotaryValue[i][curBank] = newRotaryValue;
                        // Midi output
                        if (MIDI_OUTPUT) {
                            if (i != NUM_ENCODERS-1) {
                                int cc = midiRotaryCC[i][curBank];
                                int val = map(rotaryValue[i][curBank], 0, 512, 0, 127);
                                Serial.write(0xB0);
                                Serial.write((byte)cc);
                                Serial.write((byte)val);
                                Serial.write(1);
                            }
                        }
                        if (SERIAL_OUTPUT) {
                            Serial.print("Rotary ");
                            Serial.print(i);
                            Serial.print(" = ");
                            Serial.print(map(newRotaryValue, 0, MAX_ENCODER_VAL, 0, 127));
                            Serial.println();
                        }
                    }
                } else {
                    // handles shift rotary encoder:
                    // outputs a positive or negative value depending on
                    // rotational direction. Resets encoder class value to 0
                    // to prevent value overflow in either direction.
                    if (newRotaryValue > (shiftRotaryValue + 4)) {
                        // clockwise rotation
                        rotaryEncoder[i].write(0);
                        if (MIDI_OUTPUT) {
                            int cc = midiRotaryCC[i][curBank];
                            Serial.write(0xB0);
                            Serial.write((byte)cc);
                            Serial.write("65");
                            Serial.write(1);
                        }
                        if (SERIAL_OUTPUT) {
                            Serial.print("Shift Rotary");
                            Serial.print(" = ");
                            Serial.print("+1");
                            Serial.println();
                        }
                    } else if (newRotaryValue < (shiftRotaryValue - 4)) {
                        // counter clockwise rotation
                        rotaryEncoder[i].write(0);
                        if (MIDI_OUTPUT) {
                            int cc = midiRotaryCC[i][curBank];
                            Serial.write(0xB0);
                            Serial.write((byte)cc);
                            Serial.write((byte)"63");
                            Serial.write(1);
                        }
                        if (SERIAL_OUTPUT) {
                            Serial.print("Shift Rotary");
                            Serial.print(" = ");
                            Serial.print("-1");
                            Serial.println();
                        }
                    }
                }
            }
        }
    }
}

// TODO: get buttons working
// TODO: Add serial output for debugging
// TODO: Add serial output for midi
void buttonHandler() {
    // set shift mode
    if (io.digitalRead(buttonPin[4]) == 1) {
        shiftMode = true;
    }
    else {
        shiftMode = false;
    }
    // check for shift mode
    if (shiftMode) {
        for (int i = 0; i (NUM_ENCODERS-1); i++) {
            if (io.digitalRead(buttonPin[i]) == 1) {
                curBank = i;
            }
        }
    } else {
        // cycle through all 4 buttons on the current bank to check for changes.
        for (int i = 0; i < (NUM_ENCODERS-1); i++) {
            int newButtonValue = io.digitalRead(buttonPin[i]);
            if (buttonValue[i][curBank] != newButtonValue) {
                buttonValue[i][curBank] = newButtonValue;
            }
            if (MIDI_OUTPUT) {
                int cc = midiButtonCC[i][curBank];
                int val = buttonValue[i][curBank];
                Serial.write(0xB0);
                Serial.write((byte)cc);
                Serial.write((byte)val);
                Serial.write(1);
            }
            if (SERIAL_OUTPUT) {
                Serial.print("Button ");
                Serial.print(i);
                Serial.print(" = ");
                Serial.print(buttonValue[i]);
                Serial.println();
            }
        }
    }
}

// TODO: verify this works
void ledBoot() {
    // button and rotary led sequence
    for (int i = 0; i < (NUM_ENCODERS - 1); i++) {
        for (int j = 0; j < 255; j++) {
            if (j <= 127) {
                io.analogWrite(rotaryLEDPin[i], map(j, 0, 127, 0, 255));
                io.analogWrite(buttonLEDPin[i], map(j, 0, 127, 0, 255));
            }
            else {
                io.analogWrite(rotaryLEDPin[i], map(j, 128, 255, 255, 0));
                io.analogWrite(buttonLEDPin[i], map(j, 128, 255, 255, 0));
            }
            delay(1);
        }
        io.analogWrite(rotaryLEDPin[i], 0);
        io.analogWrite(buttonLEDPin[i], 0);
    }

}

// TODO: verify leds operate properly
// TODO: verify banking works with leds
// TODO: verify shift mode flashing leds work
void ledDisplay() {
    // check for shift mode
    if (shiftMode) {
        for (int i = 0; i < (NUM_ENCODERS-1); i++) {
            // flash leds of current bank
            if (curBank == i) {
                int curTime = millis() % 200;
                if (curTime <= 100) {
                    io.analogWrite(rotaryLEDPin[i], map(curTime, 0, 200, 0, 255));
                    io.analogWrite(buttonLEDPin[i], map(curTime, 0, 200, 0, 255));
                }
                else {
                    io.analogWrite(rotaryLEDPin[i], map(curTime, 0, 200, 255, 0));
                    io.analogWrite(buttonLEDPin[i], map(curTime, 0, 200, 255, 0));
                }
            }
        }
    } else {
        for (int i = 0; i < (NUM_ENCODERS - 1); i++ ) {
            // update rotary and button leds to reflect current machine state
            io.analogWrite(rotaryLEDPin[i], map(rotaryValue[i], 0, MAX_ENCODER_VAL, 0, 255));
            io.analogWrite(buttonLEDPin[i], map(buttonValue[i], 0, 1, 0, 255);
        }
    }
}