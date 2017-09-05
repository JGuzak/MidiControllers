/*
Name:		C1Mono.ino
Created:	4/8/2017 1:54:56 PM
Author:	Jordan Guzak
*/

#include <Encoder.h>

// constants:
const bool SERIAL_OUTPUT = true;
const bool MIDI_OUTPUT = false;

const int MAX_ENCODER_VAL = 512;
const int NUM_BANKS = 4;
const int NUM_ENCODERS = 5;

const int rotaryAPin[NUM_ENCODERS] = { 2, 7, 10, 12, 0 };
const int rotaryBPin[NUM_ENCODERS] = { 4, 8, 11, 13, 1 };
const int encoderButtonPin[NUM_ENCODERS] = { A0, A1, A2, A3, A4 };
const int encoderLEDPin[NUM_ENCODERS-1] = { 3, 5, 6, 9 };
const int encoderButtonLEDPin[NUM_ENCODERS-1] = { 0, 0, 0, 0 };

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

    // initalize led pins
    for (int i = 0; i < NUM_ENCODERS - 1; i++) {
        pinMode(encoderLEDPin[i], OUTPUT);
        pinMode(encoderButtonLEDPin[i], OUTPUT);
    }
    ledBoot();
}

void loop() {
    rotaryHandler();
    buttonHandler();
    ledDisplay();
}

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
                if (i <= (NUM_ENCODERS-1)) {
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
                            if (i != NUM_ENCODERS) {
                                int cc = midiRotaryCC[i][curBank];
                                int val = rotaryValue[i][curBank];
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
                        rotaryEncoder[i].write(0);
                        if (MIDI_OUTPUT) {
                            // output positive value
                        }
                        if (SERIAL_OUTPUT) {
                            Serial.print("Shift Rotary");
                            Serial.print(" = ");
                            Serial.print("+1");
                        }
                    } else if (newRotaryValue < (shiftRotaryValue - 4)) {
                        rotaryEncoder[i].write(0);
                        if (MIDI_OUTPUT) {
                            // output negative value
                        }
                        if (SERIAL_OUTPUT) {
                            Serial.print("Shift Rotary");
                            Serial.print(" = ");
                            Serial.print("-1");
                        }
                    }
                }
            }
        }
    }
}

void buttonHandler() {
    // set shift mode
    if (digitalRead(encoderButtonPin[4]) == 1) {
        shiftMode = true;
    }
    else {
        shiftMode = false;
    }

    // check for shift mode
    if (shiftMode) {
        for (int i = 0; i (NUM_ENCODERS-1); i++) {
            if (digitalRead(encoderButtonPin[i]) == 1) {
                curBank = i;
            }
        }
    } else {
        // TODO: get buttons working
        // cycle through all 4 buttons on the current bank to check for changes.
        for (int i = 0; i < (NUM_ENCODERS-1); i++) {
            // int newButtonValue = digitalRead(encoderButtonPin[i]);
            // if (buttonValue[i][curBank] != newButtonValue) {
            //     buttonValue[i][curBank] = newButtonValue;
            // }

            // TODO: Add serial output for debugging
            if (MIDI_OUTPUT) {

            }

            // TODO: Add serial output for midi
            if (SERIAL_OUTPUT) {

            }
        }
    }
}

void ledBoot() {
    // button led sequence
    for (int i = 0; i < (NUM_ENCODERS-1); i++) {
        digitalWrite(buttonLEDPin[i], 1);
        delay(100);
    }
    for (int i = 0; i < (NUM_ENCODERS-1); i++) {
        digitalWrite(buttonLEDPin[i], 0);
    }

    // rotary led sequence
    for (int i = 0; i < (NUM_ENCODERS - 1); i++) {
        for (int j = 0; j < 255; j++) {
            if (j <= 127) {
                analogWrite(encoderLEDPin[i], map(j, 0, 127, 0, 255));
            }
            else {
                analogWrite(encoderLEDPin[i], map(j, 128, 255, 255, 0));
            }
            delay(1);
        }
        digitalWrite(encoderLEDPin[i], 0);
    }

}

void ledFlash(int analogPin, int digitalPin) {
    if (millis() % 200 > 100) {
        analogWrite(analogPin, 127);
        digitalWrite(digitalPin, HIGH);
    } else {
        analogWrite(analogPin, 0);
        digitalWrite(digitalPin, LOW);
    }
}

void ledDisplay() {
    // check for shift mode
    if (shiftMode) {
        for (int i = 0; i < (NUM_ENCODERS-1); i++) {
            // flash leds of current bank
            if (curBank == i) {
                ledFlash(encoderLEDPin[i], encoderButtonLEDPin[i])
            }
        }
    } else {
        for (int i = 0; i < (NUM_ENCODERS - 1); i++ ) {
            // update rotary leds
            analogWrite(encoderLEDPin[i], map(rotaryValue[i], 0, MAX_ENCODER_VAL, 0, 127));
            // update button leds
            digitalWrite(encoderButtonLEDPin[i], buttonValue[i]);
        }
    }
}