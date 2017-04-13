// Jordan Guzak
// Handler functions for buttons

#ifndef BUTTON_HANDLER_H
#define BUTTON_HANDLER_H

// *************************************************
// constants:
const bool BUS_DEBUG = false; // buttonUpdateState(data[])

/* for any button, each row contains:
* index | value
*  0 | pin
*  1 | type (toggle=0/momentary=1)
*  2 | state
*  3 | channel
*  4 | CC
*/
const int BUTTON_ARRAY_SIZE = 5;

/* R: array of button data
* M: nothing
* E: sends midi signal via serial port
*/
void buttonGenerateMidi(int data[]) {
    int channel = data[3];
    int cc = data[4];
    int value = data[2];

    String midiMessage = String(channel, HEX) + " " + String(cc, HEX) + " " + String(value, HEX);
    Serial.println(midiMessage);
}

/* R: button pin
* M: nothing
* E: true if button is pressed, false if button is released
*/
bool buttonGetState(int pin) {
    if (!digitalRead(pin) == 1) {
        return true;
    }
    else {
        return false;
    }
}

/* R: array of button data
* M: state of button
* E: returns button data
*/
int* buttonUpdateState(volatile int temp[]) {
    static int data[BUTTON_ARRAY_SIZE];

    for (int i = 0; i < BUTTON_ARRAY_SIZE; i++) {
        data[i] = temp[i];
    }

    bool state = buttonGetState(data[0]);

    if (state) {
        // swap state
        if (data[2] == 0) {
            data[2] = 1;
        }
        else {
            data[2] = 0;
        }

        // for debugging
        if (BUS_DEBUG) {
            Serial.println("click");
        }

        buttonGenerateMidi(data);
    }
    else {
        //  swaps state and sends another midi signal
        //  if button is momentary
        if (data[1] == 1) {
            // swap state
            if (data[2] == 0) {
                data[2] = 1;
            }
            else {
                data[2] = 0;
            }

            // for debugging
            if (BUS_DEBUG) {
                Serial.println("clack");
            }

            buttonGenerateMidi(data);
        }
    }

    return data;
}

#endif // !BUTTON_HANDLER_H