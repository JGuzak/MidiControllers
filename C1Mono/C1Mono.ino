/*
 Name:		C1Mono.ino
 Created:	4/8/2017 1:54:56 PM
 Author:	user
*/

#include <Encoder.h>

// constants:
const static int numEnocders = 5;

const static int rotaryAPin[numEnocders] = { 44, 46, 48, 50, 52 };
const static int rotaryBPin[numEnocders] = { 45, 47, 49, 51, 53 };

static long rotaryValue[numEnocders] = {0, 0, 0, 0 };

const static int encoderButtonPin[numEnocders] = { 6, 40, 41, 42, 43 };

const static int encoderLEDPin[numEnocders] = { 8, 9, 10, 11 };
const static int encoderButtonLEDPin[numEnocders] = { 2, 3, 4, 5 };

Encoder rotary1(rotaryAPin[0], rotaryBPin[0]);
Encoder rotary2(rotaryAPin[1], rotaryBPin[1]);
Encoder rotary3(rotaryAPin[2], rotaryBPin[2]);
Encoder rotary4(rotaryAPin[3], rotaryBPin[3]);
Encoder rotary5(rotaryAPin[4], rotaryBPin[4]);

static Encoder rotaryEncoder[numEnocders] = { rotary1, rotary2, rotary3, rotary4, rotary5 };


void setup() {
    Serial.begin(56000);
}


void loop() {
    long newRotaryValue;

    // read in new values
    for (int i = 0; i < numEnocders; i++) {
        newRotaryValue = rotaryEncoder[i].read();
        
        if (newRotaryValue != rotaryValue[i]) {
            Serial.println(newRotaryValue);

            rotaryValue[i] = newRotaryValue;
        }

        if (Serial.available()) {
            Serial.read();
            Serial.println("Reset knob to zero");
            rotaryEncoder[i].write(0);

        }
    }

    
  
}
