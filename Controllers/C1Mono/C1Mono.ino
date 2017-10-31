// Set Arduino.json "sketch": "Controllers\\C1Mono\\C1Mono.ino"


/*
Author:	Jordan Guzak
Created:	4/8/2017 1:54:56 PM

-------------------------
Hardware Pin Connections:

------------
Arduino Pins:
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
All others: N/A

------------
I/O Expander Pins:
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

-------------------------
Controller Logic:
TODO: WRITE THIS

Boot Sequence:

Step 1: Rotary State Updates

Step 2: Button state updates

Step 3: LED outputs

Step 4: Incoming midi messages

*/

#define ENCODER_DO_NOT_USE_INTERRUPTS
#include <Wire.h>
#include <SparkFunSX1509.h>
#include <Encoder.h>

// ---------------------------------------------------------------------
// Controller state constants:
const bool SERIAL_OUTPUT = true;

const int MAX_ENCODER_VAL = 512;
const int NUM_BANKS = 4;
const int NUM_ENCODERS = 5;
const int MAX_PRECISION = 32;
const int MIN_PRECISION = 1;

const uint8_t rotaryAPin[NUM_ENCODERS] = { 7, 9, 11, 13, 5 };
const uint8_t rotaryBPin[NUM_ENCODERS] = { 6, 8, 10, 12, 4 };
const byte buttonPin[NUM_ENCODERS] = { 12, 13, 14, 15, 11 };
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
const int shiftRotaryCC = 26;
const int midiRotaryCC[NUM_BANKS][NUM_ENCODERS-1] = {
    { 10, 11, 12, 13 },
    { 14, 15, 16, 17 },
    { 18, 19, 20, 21 },
    { 22, 23, 24, 25 }
};
const int midiButtonCC[NUM_BANKS][NUM_ENCODERS-1] = {
    { 27, 28, 29, 30 },
    { 31, 32, 33, 34 },
    { 35, 36, 37, 38 },
    { 39, 40, 41, 42 }
};

// ---------------------------------------------------------------------
// Controller state containers:
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
volatile bool buttonState[NUM_ENCODERS] = { false };
volatile int rotaryPrecision = 32;
volatile bool shiftMode = false;
volatile int curBank = 0, prevBank = 0;

void setup() {
    Serial.begin(31250);
    // Call io.begin(<address>) to initialize the SX1509. If it 
    // successfully communicates, it'll return 1.
    if (!io.begin(SX1509_ADDRESS)) 
    {
        while (1) ; // If we fail to communicate, loop forever.
    }
    // initalize led and button pins on i/o expander board
    for (int i = 0; i < NUM_ENCODERS; i++) {
        if (i < (NUM_ENCODERS - 1)) {
            io.pinMode(buttonPin[i], INPUT_PULLUP);
            if ( i <= (NUM_ENCODERS-1)) {
                io.pinMode(rotaryLEDPin[i], ANALOG_OUTPUT);
                io.pinMode(buttonLEDPin[i], ANALOG_OUTPUT);
            }
        } else {
            io.pinMode(buttonPin[i], INPUT_PULLUP);
        }
    }
    ledBoot();
}

void loop() {
    rotaryHandler();
    buttonHandler();
    ledDisplay();
    if (Serial.peek() == 0xB0) {
        receiveMidi();
    }
}

// ---------------------------------------------------------------------
// Midi handler functions:
// TODO:
//  [ ] incoming midi messages
//  [X] outgoing midi messages

/*
 * Handles incoming midi messages:
 *  CC values 10 - 25 are banked rotary incoming
 *                                values changes.
 *  CC values 27 - 42 are banked button incoming
 *                                 value changes.
 *  Rejects partial midi messages.
 */
void receiveMidi() {
    byte cmdByte, channelByte, ccByte, valueByte;
    
    if (Serial.available() > 2) {
        cmdByte = Serial.read();
        ccByte = Serial.read();
        valueByte = Serial.read();
        channelByte = Serial.read();

        if (cmdByte == 0xB0 && channelByte == 1) {
            if (ccByte >= 10 && ccByte <= 25) {
                handleRotaryMidi(ccByte, valueByte);
            } else if (ccByte >= 27 && ccByte <= 42) {
                handleButtonMidi(ccByte, valueByte);
            }
        }
    }
}

/*
 * ------------------------------
 * Banked button signal handlers:
 *  Outputs midi and serial signals.
 *  Called on banked button state changes.
 */

/**
 * Sends midi output for a button value.
 */
 void sendButtonMidi(int index) {
    int cc = midiButtonCC[curBank][index];
    int val = buttonValue[curBank][index];
    Serial.write(0xB0);
    Serial.write((byte)cc);
    Serial.write((byte)val);
}

/**
 * Sends serial output for a button value.
 */
void sendButtonSerial(int index) {
    Serial.print("Button ");
    Serial.print(index);
    Serial.print(" = ");
    Serial.print(buttonValue[curBank][index]);
    Serial.println();
}

/**
 * Updates controller state vale for a given button cc.
 * Handles bad values properly.
 */
void handleButtonMidi(int cc, int value) {
    bool ccError = false;
    for (int y = 0; y < (NUM_BANKS); y++) {
        for (int x = 0; x < (NUM_ENCODERS - 1); y++) {
            if (cc == midiButtonCC[y][x]) {
                buttonValue[y][x] = value;
            } else {
                ccError = true;
                break;
            }
        }
    }

    if (ccError && SERIAL_OUTPUT) {
        Serial.println();
        Serial.print("Error reading cc value = ");
        Serial.print(cc);
        Serial.println();
    }
}

/*
 * ------------------------------
 * Banked rotary signal handlers:
 * 
 * Outputs midi and serial signals.
 * Called on banked rotary state changes.
 */

/**
 * Sends midi output for a rotary value.
 */
void sendRotaryMidi(int index) {
    int cc = midiRotaryCC[curBank][index];
    int val = map(rotaryValue[curBank][index], 0, 512, 0, 127);
    Serial.write(0xB0);
    Serial.write((byte)cc);
    Serial.write((byte)val);
}

/**
 * Sends serial output for a rotary value.
 */
void sendRotarySerial(int index) {
    Serial.print("Rotary ");
    Serial.print(index);
    Serial.print(" = ");
    Serial.print(rotaryValue[curBank][index]);
    Serial.println();
}

/**
 * Updates controller state vale for a given rotary cc.
 * Handles bad values properly.
 */
void handleRotaryMidi(int cc, int value) {
    bool ccError = false;
    for (int y = 0; y < (NUM_BANKS); y++) {
        for (int x = 0; x < (NUM_ENCODERS - 1); y++) {
            if (cc == midiRotaryCC[y][x]) {
                rotaryValue[y][x] = value;
            } else {
                ccError = true;
                break;
            }
        }
    }

    if (ccError && SERIAL_OUTPUT) {
        Serial.println();
        Serial.print("Error reading cc value = ");
        Serial.print(cc);
        Serial.println();
    }
}

/*
 * ------------------------------
 * Shift rotary signal handlers:
 * 
 * Outputs midi and serial signals.
 * Called on shift rotary state changes.
 */

/**
 * Sends midi output for the shift rotary value.
 */
void sendShiftRotaryMidi(bool positiveOutput) {
    if (positiveOutput) {
        int cc = shiftRotaryCC;
        Serial.write(0xB0);
        Serial.write((byte)cc);
        Serial.write("65");
    } else {
        int cc = shiftRotaryCC;
        Serial.write(0xB0);
        Serial.write((byte)cc);
        Serial.write("63");
    }
}

/**
 * Sends serial output for the shift rotary value.
 */
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
// Rotary handler functions:
// TODO:
//  [ ] encoders are backwards
//  [X] added shift encoder feature for changing encoder
//                      sensitivity while in shift mode.

/**
 * Updates the encoder object values with the current
 * bank of values.
 */
void updateRotaryStates() {
    for (int i = 0; i < (NUM_ENCODERS-1); i++) {
        rotaryEncoder[i].write(rotaryValue[curBank][i]);
    }
}

/**
 * Updates the banked rotary encoder precision
 * 
 * !FOR A DELTA ENCODER!
 * (THIS WILL RESET THE STORED ENCODER VALUE TO ZERO)
 */
void updateRotaryPrecision(Encoder* encoder) {
    int newVal = encoder->read();
    if (newVal > 2) {
        // clockwise rotation
        encoder->write(0);
        if ((rotaryPrecision + 2) <= MAX_PRECISION) {
            rotaryPrecision += 2;
        } else if ((rotaryPrecision + 2) > MAX_PRECISION) {
            rotaryPrecision = MAX_PRECISION;
        }
    } else if (newVal < -2) {
        // counter clockwise rotation
        encoder->write(0);
        if ((rotaryPrecision - 2) >= MIN_PRECISION) {
            rotaryPrecision -= 2;
        } else if ((rotaryPrecision - 2) < MIN_PRECISION) {
            rotaryPrecision = MIN_PRECISION;
        }
    }
}

/**
 * Checks rotary hardware and updates various state
 * parameters.
 * 
 * Logic Outline:
 * When in shift mode:
 *  Updates the precision scaler with the shift rotary
 *  encoder.
 * 
 * When not in shift mode:
 *  Checks if the current bank state has changed. If it
 *  changed, updates state of rotary objects for the new 
 *  current bank.
 * 
 *  Checks all banked rotary encoders for new states and
 *  updates the coresponding controller state values with
 *  a scalable parameter for adjusting encoder step 
 *  precision.
 *  Outputs proper midi messages on state changes.
 * 
 *  Checks the shift encoder and outputs midi.
 */
void rotaryHandler() {
    // check for shift mode
    if (shiftMode) {
        updateRotaryPrecision(&rotaryEncoder[4]);
    }
    else {
        // sets the rotary encoders to the proper bank values and cycles
        // through all 4 rotaries on the selected bank and check for changes.
        if (prevBank != curBank) {
            updateRotaryStates();
            prevBank = curBank;
        }
        
        for (int i = 0; i < NUM_ENCODERS; i++) {
            int newVal = rotaryEncoder[i].read();
            if (i < 4) {
                if (newVal >= (rotaryValue[curBank][i] + 2)) {
                    newVal += (int)(MAX_PRECISION * (rotaryPrecision / MAX_PRECISION));
                    if (newVal >= MAX_ENCODER_VAL) {
                        rotaryEncoder[i].write(MAX_ENCODER_VAL);
                        rotaryValue[curBank][i] = MAX_ENCODER_VAL;
                        // sendRotarySerial(i);
                        sendRotaryMidi(i);
                    } else {
                        rotaryEncoder[i].write(newVal);
                        rotaryValue[curBank][i] = newVal;
                        // sendRotarySerial(i);
                        sendRotaryMidi(i);
                    }
                } else if (newVal <= (rotaryValue[curBank][i] - 2)) {
                    newVal -= (int)(MAX_PRECISION * (rotaryPrecision / MAX_PRECISION));
                    if (newVal <= 0) {
                        rotaryEncoder[i].write(0);
                        rotaryValue[curBank][i] = 0;
                        // sendRotarySerial(i);
                        sendRotaryMidi(i);
                    } else {
                        rotaryEncoder[i].write(newVal);
                        rotaryValue[curBank][i] = newVal;
                        // sendRotarySerial(i);
                        sendRotaryMidi(i);
                    }
                }
            } else {
                if (newVal > 4) {
                    // clockwise rotation
                    rotaryEncoder[i].write(0);
                    // sendShiftRotarySerial(true);
                    sendShiftRotaryMidi(true);
                } else if (newVal < -4) {
                    // counter clockwise rotation
                    rotaryEncoder[i].write(0);
                    // sendShiftRotarySerial(false);
                    sendShiftRotaryMidi(false);
                }
            }
        }

    }
}

// ---------------------------------------------------------------------
// button handler functions:

/**
 * Updates shift mode based on shift rotary button.
 * This does not output a midi signal on state change.
 */
void shiftButtonStateHandler() {
    int newButtonValue = io.digitalRead(buttonPin[4]);
    if (newButtonValue == 0) {
        shiftMode = true;
    } else if (newButtonValue == 1) {
        shiftMode = false;
    }
}

/**
 * Updates the controller state for banked rotary i.
 * Outputs midi signal for coresponding change.
 */
void bankButtonStateHandler(int i) {
    int newButtonValue = io.digitalRead(buttonPin[i]);
    if (newButtonValue == 0 && !buttonState[i]) {
        buttonState[i] = true;
        if (buttonValue[curBank][i] == 0) {
            buttonValue[curBank][i] = 1;
        } else {
            buttonValue[curBank][i] = 0;
        }
        
        // sendButtonSerial(i);
        sendButtonMidi(i);
    } else if (newButtonValue == 1 && buttonState[i]) {
        buttonState[i] = false;
    }
}

/**
 * Checks button hardware and updates various state
 * parameters.
 * 
 * Logic Outline:
 * Checks for shift mode change and updates controller
 * state.
 * When in shift mode:
 *  Checks button states to update current bank state.
 * 
 * When not in shift mode:
 *  Checks all banked buttons for new states and updates
 *  the coresponding controller state values.
 *  Outputs proper midi messages on state changes.
 */
void buttonHandler() {
    // set shift mode
    shiftButtonStateHandler();

    // check for shift mode
    if (shiftMode) {
        for (int i = 0; i < (NUM_ENCODERS-1); i++) {
            if (io.digitalRead(buttonPin[i]) == 0) {
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
// TODO:
//  [ ] Document LED test function

/**
 * Runs a light animation used in the boot sequence
 * and as a screensaver.
 * 
 * Warnings:
 *  Takes ~1.02 seconds to return. This function
 *  is not suitable for time sensitive sequences!
 */
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

/**
 * flashes two leds on I/O expander on pin a
 * a = byte with a value between 0-15 (for pins 0-15)
 */
void ledFlash(byte a) {
    float time = 1100 - (((float)rotaryPrecision / (float)MAX_PRECISION) * 900);
    // float time = 200;
    int curTime = millis() % (int)time;
    if (curTime <= (int)(time / 2)) {
        io.analogWrite(a, map(curTime, 0, (int)time, 255, 0));
    }
    else {
        io.analogWrite(a, map(curTime, 0, (int)time, 0, 255));
    }
}


/**
 * Sets an led brightness for the given rotaryPin on
 * I/O expander with a rotary value between 0 and MAX_ENCODER_VAL.
 */
void setLedRotaryState(byte rotaryPin, int index) {
    io.analogWrite(rotaryPin, map(rotaryValue[curBank][index], 0, MAX_ENCODER_VAL, 255, 0));
}

/**
 * Sets and led brightness for the given buttonPin on
 * I/O expander either on or off.
 */
void setLedButtonState(byte buttonPin, int index) {
    io.analogWrite(buttonPin, map(buttonValue[curBank][index], 0, 1, 255, 0));
}

/**
 * Turns off all leds.
 */
void ledClear() {
    for (int i = 0; i < (NUM_ENCODERS - 1); i++) {
        io.analogWrite(rotaryLEDPin[i], 255);
        io.analogWrite(buttonLEDPin[i], 255);
    }
}

/**
 * Sets led values depending on:
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

/**
 * 
 */
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
