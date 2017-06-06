/*
Name:		C1Mono.ino
Created:	4/8/2017 1:54:56 PM
Author:	Jordan Guzak
*/

#include <Encoder.h>

// constants:
const bool ROTARY_CHANGE_DEBUG = true;
const int MAX_ENCODER_VAL = 512;

const int numEnocders = 5;

const int rotaryAPin[numEnocders] = { 2, 7, 10, 12, 0 };
const int rotaryBPin[numEnocders] = { 4, 8, 11, 13, 1 };
const int encoderButtonPin[numEnocders] = { A0, A1, A2, A3, A4 };

const int encoderLEDPin[numEnocders] = { 3, 5, 6, 9 };
const int encoderButtonLEDPin[numEnocders] = { 2, 3, 4, 5 };

static Encoder rotaryEncoder[numEnocders] = {
    { rotaryAPin[0], rotaryBPin[0] },
    { rotaryAPin[1], rotaryBPin[1] },
    { rotaryAPin[2], rotaryBPin[2] },
    { rotaryAPin[3], rotaryBPin[3] },
    { rotaryAPin[4], rotaryBPin[4] }
};

volatile long rotaryValue[numEnocders] = { 0, 0, 0, 0 };
volatile int buttonValue[numEnocders] = { 0, 0, 0, 0 };

void setup() {
    Serial.begin(56000);


}


void loop() {

    // rotary encoder value update
    for (int i = 0; i < numEnocders; i++) {
        long newRotaryValue;
        newRotaryValue = rotaryEncoder[i].read();

        // check for rotary change, do nothing otherwise
        if ( (newRotaryValue >= (rotaryValue[i] + 4)) || (newRotaryValue <= (rotaryValue[i] - 4)) ) {
            // check for min/max value, update value otherwise
            if ( newRotaryValue < 0 ) {
                rotaryEncoder[i].write(0);
            }
            else if ( newRotaryValue > MAX_ENCODER_VAL ) {
                rotaryEncoder[i].write(MAX_ENCODER_VAL);
            }
            else {
                rotaryEncoder[i].write(newRotaryValue);
                rotaryValue[i] = newRotaryValue;

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

    if (Serial.available()) {
        Serial.read();
        Serial.println("Reset knob to zero");
        for (int j = 0; j < numEnocders; j++) {
            rotaryEncoder[j].write(0);
            rotaryValue[j] = 0;
        }
    }

    
}