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
#define ENCODER_DO_NOT_USE_INTERRUPTS
#include <Encoder.h>

// State Constants:
const bool SERIAL_OUTPUT = true;
const bool MIDI_OUTPUT = false;

const int MAX_ENCODER_VAL = 512;
const int NUM_BANKS = 4;
const int NUM_ENCODERS = 5;

const uint8_t rotaryBPin[NUM_ENCODERS] = { 7, 9, 11, 13, 5 };
const uint8_t rotaryAPin[NUM_ENCODERS] = { 6, 8, 10, 12, 4 };
const byte buttonPin[NUM_ENCODERS] = { 12, 13, 14, 15, 11 };
const byte rotaryLEDPin[NUM_ENCODERS-1] = { 6, 4, 2, 0 };
const byte buttonLEDPin[NUM_ENCODERS-1] = { 7, 5, 3, 1 };

const byte SX1509_ADDRESS = 0x3E;
SX1509 io;

Encoder rotaryEncoder[NUM_ENCODERS] = {
    Encoder(rotaryAPin[0], rotaryBPin[0]),
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
volatile bool buttonState[NUM_ENCODERS] = { false };
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
    for (int i = 0; i < (NUM_ENCODERS - 1); i++) {
        io.pinMode(buttonPin[i], INPUT_PULLUP);
        if ( i <= (NUM_ENCODERS-1)) {
            io.pinMode(rotaryLEDPin[i], ANALOG_OUTPUT);
            io.pinMode(buttonLEDPin[i], ANALOG_OUTPUT);
        }
    }
    ledBoot();
}

void loop() {
    // for debugging only
    // ledBoot();
    // ledDisplayTest();
    rotaryTest();

    // rotaryHandler();
    // buttonHandler();
    ledDisplay();
}

// ---------------------------------------------------------------------
// 
void updateControllerState() {
    for (int i = 0; i < (NUM_ENCODERS-1); i++) {
        rotaryEncoder[i].write(rotaryValue[curBank][i]);
    }
}

// ---------------------------------------------------------------------
// midi handler functions:
// TODO:
//  [ ] incoming midi messages
//  [ ] outgoing midi messages
void receiveMidi() {
    
}

// sending button signals
void sendButtonMidi(int index) {
    int cc = midiButtonCC[bank][index];
    int val = buttonValue[bank][index];
    Serial.write(0xB0);
    Serial.write((byte)cc);
    Serial.write((byte)val);
    Serial.write(1);
}

void sendButtonSerial(int index) {
    Serial.print("Button ");
    Serial.print(i);
    Serial.print(" = ");
    Serial.print(buttonValue[curBank][i]);
    Serial.println();
}

// sending banked rotary signals
void sendRotaryMidi(int index) {
    int cc = midiRotaryCC[curBank][index];
    int val = map(rotaryValue[curBank][index], 0, 512, 0, 127);
    Serial.write(0xB0);
    Serial.write((byte)cc);
    Serial.write((byte)val);
    Serial.write(1);
}

void sendRotarySerial(int index) {
    Serial.print("Rotary ");
    Serial.print(index);
    Serial.print(" = ");
    Serial.print(map(rotaryValue[curBank][index], 0, MAX_ENCODER_VAL, 0, 127));
    Serial.println();
}

// sending shift rotary signals
void sendShiftRotaryMidi(bool positiveOutput) {
    if (positiveOutput) {
        int cc = midiRotaryCC[curBank][i];
        Serial.write(0xB0);
        Serial.write((byte)cc);
        Serial.write("65");
        Serial.write(1);
    } else {
        int cc = midiRotaryCC[curBank][i];
        Serial.write(0xB0);
        Serial.write((byte)cc);
        Serial.write("63");
        Serial.write(1);
    }
}

void sendShiftRotarySerial(bool positiveOutput) {
    if (positiveOutput) {
        Serial.print("Shift Rotary");
        Serial.print(" = ");
        Serial.print("+1");
        Serial.println();
    } else {
        Serial.print("Shift Rotary");
        Serial.print(" = ");
        Serial.print("-1");
        Serial.println();
    }
}

// ---------------------------------------------------------------------
// rotary handler functions:
// TODO:
//  [ ] encoders are backwards
void rotaryHandler() {
    // check for shift mode
    if (shiftMode) {
    }
    else {
        // sets the rotary encoders to the proper bank values and cycles
        // through all 4 rotaries on the selected bank and check for changes.
        updateControllerState();
        
        for (int i = 0; i < NUM_ENCODERS; i++) {
            int newVal = rotaryEncoder[i].read();
            if (i < 4) {
                if (newVal > (rotaryValue[curBank][i] + 3) || newVal < (rotaryValue[curBank][i] - 3)) {
                    if (newVal < 0) {
                        rotaryEncoder[i].write(0);
                        rotaryValue[curBank][i] = 0;
                    } else if (newVal > MAX_ENCODER_VAL) {
                        rotaryEncoder[i].write(MAX_ENCODER_VAL);
                        rotaryValue[curBank][i] = MAX_ENCODER_VAL;
                    } else {
                        rotaryEncoder[i].write(newVal);
                        rotaryValue[curBank][i] = newVal;
                        if (MIDI_OUTPUT) {
                            sendRotaryMidi(i);
                        }
                        if (SERIAL_OUTPUT) {
                            sendRotarySerial(i);
                        }
                    }
                }
            } else {
                if (newVal > (shiftRotaryValue + 3)) {
                    // clockwise rotation
                    rotaryEncoder[i].write(0);
                    if (MIDI_OUTPUT) {
                        sendShiftRotaryMidi(true);
                    }
                    if (SERIAL_OUTPUT) {
                        sendShiftRotarySerial(true);
                    }
                } else if (newVal < (shiftRotaryValue - 3)) {
                    // counter clockwise rotation
                    rotaryEncoder[i].write(0);
                    if (MIDI_OUTPUT) {
                        sendShiftRotaryMidi(false);
                    }
                    if (SERIAL_OUTPUT) {
                        sendShiftRotarySerial(false);
                    }
                }
            }
        }

    }
}

// ---------------------------------------------------------------------
// button handler functions:
void shiftButtonStateHandler() {
    int newButtonValue = io.digitalRead(buttonPin[NUM_ENCODERS-1]);
    if (newButtonValue == 0) {
        shiftMode = true;
    } else if (newButtonValue == 1) {
        shiftMode = false;
    }
}

void bankButtonStateHandler(int i) {
    int newButtonValue = io.digitalRead(buttonPin[i+1]);
    if (newButtonValue == 0 && !buttonState[i+1]) {
        buttonState[i+1] = true;
        if (buttonValue[curBank][i] == 0) {
            buttonValue[curBank][i] = 1;
        } else {
            buttonValue[curBank][i] = 0;
        }
        if (MIDI_OUTPUT) {
            sendButtonMidi(i);
        }
        if (SERIAL_OUTPUT) {
            sendButtonSerial(i);
        }
    } else if (newButtonValue == 1 && buttonState[i+1]) {
        buttonState[i+1] = false;
    }
}

void buttonHandler() {
    // set shift mode
    shiftButtonStateHandler();

    // check for shift mode
    if (shiftMode) {
        for (int i = 0; i < (NUM_ENCODERS-1); i++) {
            if (io.digitalRead(buttonPin[i+1]) == 0) {
                curBank = i;
            }
        }
    } else {
        // cycle through all 4 buttons on the current bank to check for changes.
        for (int i = 0; i < (NUM_ENCODERS - 1); i++) {
            bankButtonStateHandler(i);
        }
    }
}

// ---------------------------------------------------------------------
// led handler functions:
void ledBoot() {
    // button and rotary led sequence
    // raises and lowers brightness of column of leds from
    // left to right.
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

// flashes two leds on I/O expander pin a
void ledFlash(byte a) {
    int curTime = millis() % 200;
    if (curTime <= 100) {
        io.analogWrite(a, map(curTime, 0, 200, 255, 0));
    }
    else {
        io.analogWrite(a, map(curTime, 0, 200, 0, 255));
    }
}

void setLedRotaryState(byte rotaryPin, int index) {
    io.analogWrite(rotaryPin, map(rotaryValue[curBank][index], 0, MAX_ENCODER_VAL, 255, 0));
}

void setLedButtonState(byte buttonPin, int index) {
    io.analogWrite(buttonPin, map(buttonValue[curBank][index], 0, 1, 255, 0));
}

/**
 * Turns off all rotary and button leds.
 */
void ledClear() {
    for (int i = 0; i < (NUM_ENCODERS - 1); i++) {
        io.analogWrite(rotaryLEDPin[i], 255);
        io.analogWrite(buttonLEDPin[i], 255);
    }
}

/**
 * sets led values depending on:
 *  shift mode
 *  current bank state
 *  rotary and button values
 */
void ledDisplay() {
    ledClear();
    // check for shift mode
    if (shiftMode) {
        ledFlash(rotaryLEDPin[curBank]);
        ledFlash(buttonLEDPin[curBank]);
    } else {
        // update rotary and button leds to reflect current machine state
        for (int i = 0; i < (NUM_ENCODERS-1); i++ ) {
            setLedRotaryState(rotaryLEDPin[i], i);
            setLedButtonState(buttonLEDPin[i], i);
        }
    }
    ledClear();
}

void ledDisplayTest() {
    for (int i = 0; i < (NUM_ENCODERS - 1); i++) {
        shiftMode = true;
        curBank = i;
        int startTime = millis();
        while( (millis() - startTime) < 1000) {
            ledDisplay();
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
