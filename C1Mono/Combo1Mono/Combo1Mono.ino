/*
Name:		C1Mono.ino
Created:	4/8/2017 1:54:56 PM
Author:	Jordan Guzak
*/

#include <Encoder.h>

// constants:
const bool ROTARY_CHANGE_DEBUG = false;
const bool ROTARY_MIDI_OUTPUT = true;
const int MAX_ENCODER_VAL = 512;

const int numEnocders = 5;

const int rotaryAPin[numEnocders] = { 2, 7, 10, 12, 0 };
const int rotaryBPin[numEnocders] = { 4, 8, 11, 13, 1 };
const int encoderButtonPin[numEnocders] = { A0, A1, A2, A3, A4 };

const int encoderLEDPin[numEnocders-1] = { 3, 5, 6, 9 };
const int encoderButtonLEDPin[numEnocders-1] = { 0, 0, 0, 0 };

Encoder rotaryEncoder[numEnocders] = {
    { rotaryAPin[0], rotaryBPin[0] },
    { rotaryAPin[1], rotaryBPin[1] },
    { rotaryAPin[2], rotaryBPin[2] },
    { rotaryAPin[3], rotaryBPin[3] },
    { rotaryAPin[4], rotaryBPin[4] }
};

const int midiRotaryCC[numEnocders-1] = { 10, 11, 12, 13 };
volatile int rotaryValue[numEnocders] = { 0, 0, 0, 0, 0 };
volatile int buttonValue[numEnocders] = { 0, 0, 0, 0, 0 };

void setup() {
    Serial.begin(56000);

    // initalize led pins
    for (int i = 0; i < numEnocders - 1; i++) {
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
    if (buttonValue[4] == 1) {

    }
    else {
        for (int i = 0; i < numEnocders; i++) {
            long newRotaryValue;
            newRotaryValue = rotaryEncoder[i].read();

            // check for rotary change, do nothing otherwise
            if ((newRotaryValue >= (rotaryValue[i] + 4)) || (newRotaryValue <= (rotaryValue[i] - 4))) {
                // check for min/max value, update value otherwise
                if (newRotaryValue < 0) {
                    rotaryEncoder[i].write(0);
                }
                else if (newRotaryValue > MAX_ENCODER_VAL) {
                    rotaryEncoder[i].write(MAX_ENCODER_VAL);
                }
                else {
                    rotaryEncoder[i].write(newRotaryValue);
                    rotaryValue[i] = newRotaryValue;

                    if (ROTARY_MIDI_OUTPUT) {
                        if (i != numEnocders) {
                            int cc = midiRotaryCC[i];
                            int val = rotaryValue[i];
                            Serial.write(0xB0);
                            Serial.write((byte)cc);
                            Serial.write((byte)val);
                            Serial.write(1);
                        }
                    }

                    if (ROTARY_CHANGE_DEBUG) {
                        Serial.print("Rotary ");
                        Serial.print(i);
                        Serial.print(" = ");
                        Serial.print(map(newRotaryValue, 0, MAX_ENCODER_VAL, 0, 127));
                        Serial.println();
                    }
                }
            }
        }

        if (ROTARY_CHANGE_DEBUG) {
            if (Serial.available()) {
                Serial.read();
                Serial.println("Reset knob to zero");
                for (int j = 0; j < numEnocders; j++) {
                    rotaryEncoder[j].write(0);
                    rotaryValue[j] = 0;
                }
            }
        }
    }
}

void buttonHandler() {
    // check for shift mode
    if (buttonValue[4] == 1) {

    }
    else {
    
    }
}

void ledBoot() {
    // button led loop

    // rotary led loop
    for (int i = 0; i < (numEnocders - 1); i++) {
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

void ledDisplay() {

    // check for shift mode
    if (buttonValue[4] == 1) {

    } else {
        for (int i; i < (numEnocders - 1); i++ ) {
            // update rotary leds
            analogWrite(encoderLEDPin[i], map(rotaryValue[i], 0, MAX_ENCODER_VAL, 0, 127));
            // update button leds
            digitalWrite(encoderButtonLEDPin[i], buttonValue[i]);
        }
    }
}