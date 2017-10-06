/*
Name:		C1Mono.ino
Created:	4/8/2017 1:54:56 PM
Author:	Jordan Guzak

Pin Connections:

Arduino:
A0: N/A
A1: N/A
A2: N/A
A3: N/A
A4: N/A
A5: N/A

0: N/A
1: N/A
2: N/A
3: N/A
4: Rotary 1 A
5: Rotary 1 B
6: Rotary 2 A
7: Rotary 2 B
8: Rotary 3 A
9: Rotary 3 B
10: Rotary 4 A
11: Rotary 4 B
12: Rotary 5 A
13: Rotary 5 B

SCL: I/O Expander
SDA: I/O Expander
3.3V: I/O Expander
GND: I/O Expander

I/O Expander:
0: Button 5 LED
1: Rotary 5 LED
2: Button 4 LED
3: Rotary 4 LED
4: Button 3 LED
5: Rotary 3 LED
6: Button 2 LED
7: Rotary 2 LED
8: N/A
9: N/A
10: N/A
11: Button 5 Input
12: Button 4 Input
13: Button 3 Input
14: Button 2 Input
15: Button 1 Input

VC1: N/A
GND: Protoboard

*/

#include <Wire.h>
#include <SparkFunSX1509.h>
#include <Encoder.h>

// State Constants:
const bool SERIAL_OUTPUT = true;
const bool MIDI_OUTPUT = false;

const int MAX_ENCODER_VAL = 512;
const int NUM_BANKS = 4;
const int NUM_ENCODERS = 5;

const uint8_t rotaryAPin[NUM_ENCODERS] = { 2, 4, 6, 8, 10 };
const uint8_t rotaryBPin[NUM_ENCODERS] = { 3, 5, 7, 9, 11 };
const byte buttonPin[NUM_ENCODERS] = { 11, 12, 13, 14, 15 };
const byte rotaryLEDPin[NUM_ENCODERS-1] = { 6, 4, 2, 0 };
const byte buttonLEDPin[NUM_ENCODERS-1] = { 7, 5, 3, 1 };

const byte SX1509_ADDRESS = 0x3E;
SX1509 io;

Encoder rotaryEncoder[NUM_ENCODERS] = {
    { rotaryAPin[0], rotaryBPin[0] },
    { rotaryAPin[1], rotaryBPin[1] },
    { rotaryAPin[2], rotaryBPin[2] },
    { rotaryAPin[3], rotaryBPin[3] },
    { rotaryAPin[4], rotaryBPin[4] }
};
int midiRotaryCC[NUM_BANKS][NUM_ENCODERS] = {
    { 10, 11, 12, 13, 26 },
    { 14, 15, 16, 17, 26 },
    { 18, 19, 20, 21, 26 },
    { 22, 23, 24, 25, 26 }
};
const int midiButtonCC[NUM_BANKS][NUM_ENCODERS-1] = {
    { 27, 28, 29, 30 },
    { 31, 32, 33, 34 },
    { 35, 36, 37, 38 },
    { 39, 40, 41, 42 }
};
volatile int rotaryValue[NUM_BANKS][NUM_ENCODERS-1] = {
    { 0, 0, 0, 0 },
    { 0, 0, 0, 0 },
    { 0, 0, 0, 0 },
    { 0, 0, 0, 0 }
};
volatile int buttonValue[NUM_BANKS][NUM_ENCODERS-1] = {
    { 0, 0, 0, 0 },
    { 0, 0, 0, 0 },
    { 0, 0, 0, 0 },
    { 0, 0, 0, 0 }
};
volatile bool shiftMode = false;
volatile int curBank = 0, shiftRotaryValue = 0;

void setup() {
    Serial.begin(9600);
    // Call io.begin(<address>) to initialize the SX1509. If it 
    // successfully communicates, it'll return 1.
    if (!io.begin(SX1509_ADDRESS)) 
    {
        while (1) ; // If we fail to communicate, loop forever.
    }
    // initalize led and button pins on i/o expander board
    for (int i = 0; i < NUM_ENCODERS - 1; i++) {
        io.pinMode(buttonPin[i], INPUT_PULLUP);
        if ( i <= NUM_ENCODERS-1) {
            io.pinMode(rotaryLEDPin[i], ANALOG_OUTPUT);
            io.pinMode(buttonLEDPin[i], ANALOG_OUTPUT);
        }
    }
    ledBoot();
}

void loop() {
    // for debugging only
    // ledBoot();
    ledDisplayTest();

    // rotaryHandler();
    // buttonHandler();
}

// TODO: verify encoders work
// TODO: verify banking works
// TODO: verify shift encoder works properly
void rotaryHandler() {
    // check for shift mode
    if (shiftMode) {
    }
    else {
        // sets the rotary encoders to the proper bank values and cycles
        // through all 4 rotaries on the selected bank and check for changes.
        for (int i = 0; i < (NUM_ENCODERS-1); i++) {
            rotaryEncoder[i].write(rotaryValue[i][curBank]);
        }
        for (int i = 0; i < (NUM_ENCODERS-1); i++) {
            long newRotaryValue;
            newRotaryValue = rotaryEncoder[i].read();
            // check for rotary changes, do nothing otherwise
            // for banked rotaries
            if ((newRotaryValue >= (rotaryValue[curBank][i] + 4)) || (newRotaryValue <= (rotaryValue[curBank][i] - 4))) {
                // for banked rotary encoders:
                // check for min/max value, update value otherwise
                // handle edge cases where rotary would excede maximum and minimum values
                if (i < (NUM_ENCODERS-2)) {
                    if (newRotaryValue < 0) {
                        rotaryEncoder[i].write(0);
                    }
                    else if (newRotaryValue > MAX_ENCODER_VAL) {
                        rotaryEncoder[i].write(MAX_ENCODER_VAL);
                    }
                    else {
                        rotaryEncoder[i].write(newRotaryValue);
                        rotaryValue[curBank][i] = newRotaryValue;
                        // Midi output
                        if (MIDI_OUTPUT) {
                            if (i != NUM_ENCODERS-1) {
                                int cc = midiRotaryCC[curBank][i];
                                int val = map(rotaryValue[curBank][i], 0, 512, 0, 127);
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
                            int cc = midiRotaryCC[curBank][i];
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
                            int cc = midiRotaryCC[curBank][i];
                            Serial.write(0xB0);
                            Serial.write((byte)cc);
                            Serial.write("63");
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
    if (io.digitalRead(buttonPin[0]) == 0) {
    //     shiftMode = true;
    // }
    // else {
    //     shiftMode = false;
    }
    // check for shift mode
    if (shiftMode) {
        // for (int i = 0; i < (NUM_ENCODERS-3); i++) {
        //     if (io.digitalRead(buttonPin[i+1]) == 1) {
        //         curBank = i;
        //     }
        // }
    } else {
        // cycle through all 4 buttons on the current bank to check for changes.
        for (int i = 1; i < (NUM_ENCODERS); i++) {
            int newButtonValue = io.digitalRead(buttonPin[i]);
            if (newButtonValue == 0) {
                if (buttonValue[curBank][i-1] == 0) {
                    buttonValue[curBank][i-1] = 1;
                } else {
                    buttonValue[curBank][i-1] = 0;
                }
            }
            // if (MIDI_OUTPUT) {
            //     int cc = midiButtonCC[curBank][i-1];
            //     int val = buttonValue[curBank][i-1];
            //     Serial.write(0xB0);
            //     Serial.write((byte)cc);
            //     Serial.write((byte)val);
            //     Serial.write(1);
            // }
            if (SERIAL_OUTPUT) {
                Serial.print("Button ");
                Serial.print(i);
                Serial.print(" = ");
                Serial.print(buttonValue[curBank][i-1]);
                Serial.println();
            }
        }
    }
}

void ledBoot() {
    // button and rotary led sequence
    for (int i = 0; i < (NUM_ENCODERS - 1); i++) {
        for (int j = 0; j < 255; j++) {
            if (j <= 127) {
                io.analogWrite(rotaryLEDPin[i], map(j, 128, 255, 255, 0));
                io.analogWrite(buttonLEDPin[i], map(j, 128, 255, 255, 0));
            }
            else {
                io.analogWrite(rotaryLEDPin[i], map(j, 0, 127, 0, 255));
                io.analogWrite(buttonLEDPin[i], map(j, 0, 127, 0, 255));
            }
            delay(1);
        }
        io.analogWrite(rotaryLEDPin[i], 255);
        io.analogWrite(buttonLEDPin[i], 255);
    }

}

void ledFlash(byte rotaryPin, byte buttonPin) {
    int curTime = millis() % 200;
    if (curTime <= 100) {
        io.analogWrite(rotaryPin, map(curTime, 0, 200, 255, 0));
        io.analogWrite(buttonPin, map(curTime, 0, 200, 255, 0));
    }
    else {
        io.analogWrite(rotaryPin, map(curTime, 0, 200, 0, 255));
        io.analogWrite(buttonPin, map(curTime, 0, 200, 0, 255));
    }
}

void ledDisplayTest() {
    

    for (int i = 0; i < (NUM_ENCODERS - 1); i++) {
        shiftMode = true;
        curBank = i;
        int startTime = millis();
        while( (millis() - startTime) < 1000) {
            ledFlash(rotaryLEDPin[i], buttonLEDPin[i]);
        }

        shiftMode = false;
        ledClear();
        
        for (int v = 0; v < 1024; v++) {
            for (int i = 0; i < (NUM_ENCODERS-1); i++) {
                if (v == 512) {
                    if (buttonValue[curBank][i] == 0) {
                        buttonValue[curBank][i] = 1;
                    } else {
                        buttonValue[curBank][i] = 0;
                    }
                }

                if (v > 512) {
                    rotaryValue[curBank][i] = 1024 - v;
                } else {
                    rotaryValue[curBank][i] = v;
                }
            }
            delay(1);
            ledDisplay();
            ledClear();
        }


    }
}

void ledState(byte rotaryPin, byte buttonPin, int index) {
    io.analogWrite(rotaryPin, map(rotaryValue[curBank][index], 0, MAX_ENCODER_VAL, 255, 0));
    io.analogWrite(buttonPin, map(buttonValue[curBank][index], 0, 1, 255, 0));
}

void ledClear() {
    for (int i = 0; i < (NUM_ENCODERS - 1); i++) {
        io.analogWrite(rotaryLEDPin[i], 255);
        io.analogWrite(buttonLEDPin[i], 255);
    }
}

// TODO: verify leds operate properly
// TODO: verify banking works with leds
// TODO: verify shift mode flashing leds work
void ledDisplay() {
    // check for shift mode
    if (shiftMode) {
        // for (int i = 0; i < (NUM_ENCODERS-1); i++) {
        //     // flash leds of current bank
        //     if (curBank == i) {
        //         ledFlash(rotaryLEDPin[i], buttonLEDPin[i]);
        //     }
        // }
    } else {
 
        for (int i = 0; i < (NUM_ENCODERS-1); i++ ) {
            // update rotary and button leds to reflect current machine state
            ledState(rotaryLEDPin[i], buttonLEDPin[i], i);
        }
    }
}
