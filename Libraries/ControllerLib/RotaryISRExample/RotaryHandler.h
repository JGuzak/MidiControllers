// Jordan Guzak
// Handler funtions for rotary encoders

#ifndef ROTARY_HANDLER_H
#define ROTARY_HANDLER_H

// *************************************************
// constants:
const bool RUS_DEBUG = false; // rotaryUpdateState(int data[])
const bool RGS_DEBUG = false; // rotaryGetState(int, int)

const int ROTARY_STATES_SIZE = 4;

/* for a value based rotary, each row contains:
* index | value
*  0 | pin A
*  1 | pin B
*  2 | step
*  3 | current state
*  4 | last state
*  5 | channel
*  6 | CC
*  7 | value
*/
const int ROTARY_VALUE_ARRAY_SIZE = 8;

const int ROTARY_VALUE_MIN = 0;
const int ROTARY_VALUE_MAX = 127;

/* for a delta based rotary, each row contains:
* index | value
*  0 | pin A
*  1 | pin B
*  2 | step
*  3 | current state
*  4 | last state
*  5 | channel
*  6 | CC
*/
const int ROTARY_DELTA_ARRAY_SIZE = 7;


// int values of each state
// index | AB | uno | due
// 0 | 00 | 0 | 
// 1 | 10 | 2
// 2 | 11 | 3
// 3 | 01 | 1
const int ROTARY_STATES[4] = { 0, 2, 3, 1 };

// *************************************************
// functions:

/* R: array of value based rotary encoder data
* M: nothing
* E: sends midi signal via serial port
*/
void rotaryValGenerateMidiMessage(int data[]) {
    int channel = data[5];
    int CC = data[6];
    int val = data[7];

    String midiMessage = String(channel, HEX) + " " + String(CC, HEX) + " " + String(val, HEX);

    Serial.println(midiMessage);

}

/* R: array of delta based rotary encoder data
* M: nothing
* E: sends midi signal via serial port
*/
void rotaryDeltaGenerateMidiMessage(int data[]) {
    int channel = data[4];
    int CC = data[5];
    int val;
    
    if (data[4] == 1) {
        val = 1;
    }
    else {
        val = -1;
    }

    String midiMessage = String(channel, HEX) + " " + String(CC, HEX) + " " + String(val, HEX);

    Serial.println(midiMessage);
}

/* R: pin a and pin b
 * M: nothing
 * E: returns the index of ROTARYSTATE for the current state
 */
int rotaryGetState(int a, int b) {
    int lhs = !digitalRead(a), rhs = !digitalRead(b);

    // state should reflect the following pin -> state config
    // A|B = state index, where:
    // 0|0 = 0
    // 1|0 = 1
    // 1|1 = 2
    // 0|1 = 3
    int newState = (lhs << 1) + rhs;
    int stateIndex = -1;

    // iterates through ROTARY_STATES to find the correct 
    //  value and saves the index of matched value
    for (int i = 0; i < ROTARY_STATES_SIZE; i++) {
        if (newState == ROTARY_STATES[i]) {
            stateIndex = i;
        }
    }

    // for debugging
    if (RGS_DEBUG) {
        Serial.print(lhs);
        Serial.print("|");
        Serial.print(rhs);
        Serial.print(" = ");
        Serial.print(newState);
        Serial.print(" at index ");
        Serial.print(stateIndex);
        Serial.println();

    }

    return stateIndex;
}

/* R: array of value based rotary encoder data
 * M: state, step, and value of rotary encoder
 * E: returns rotary encoder data
 */
int* rotaryValueUpdateState(int temp[]) {
    static int data[ROTARY_VALUE_ARRAY_SIZE];

    // populate static data array for returning data
    for (int a = 0; a < ROTARY_VALUE_ARRAY_SIZE; a++) {
        data[a] = temp[a];
    }

    // read in new state
    int nextState = rotaryGetState(data[0], data[1]);

    // checks for different state and updates 
    //  step, direction, and state accordingly
    //  
    // the step counter increases by 1 if the 
    //  new state is the next state in the series
    //  and resets the counter if a state is skipped
    //  
    // needs to implement direction to catch
    //  remaining bounce states
    if (nextState != data[3]) {
        if (nextState == (data[3] + 1 % 3)) {
            data[2] += 1;
            data[4] = 1;
        }
        else if (nextState == (data[3] - 1 % 3)) {
            data[2] += 1;
            data[4] = -1;
        }
        else {
            data[2] = 0;
        }

        data[3] = nextState;
    }

    if (RUS_DEBUG) {
        if (data[2] == 3) {
            if (data[4] == 1) {
                //Serial.println("clockwise cycle");
            }
            else {
                //Serial.println("counter clockwise cycle");
            }
        }
    }


    // check for send state and update value accordingly
    if (data[2] == 3) {
        if (data[4] == 1) {
            if (ROTARY_VALUE_MIN <= data[7] && data[7] < ROTARY_VALUE_MAX) {
                data[7] += 1;
            }
        }
        else {
            if (ROTARY_VALUE_MIN < data[7] && data[7] <= ROTARY_VALUE_MAX) {
                data[7] -= 1;
            }
        }

        rotaryValGenerateMidiMessage(data);

        // for debugging rotary values
        if (RUS_DEBUG) {
            Serial.print(data[6]);
            Serial.print(" -> ");
            Serial.print(data[6]);
            Serial.println();
        }

        data[2] = 0;
    }

    return data;

}

/* R: array of delta basedrotary encoder data
* M: state, step, and value of rotary encoder
* E: returns rotary encoder data
*/
int* rotaryDeltaUpdateState(int temp[]) {
    static int data[ROTARY_DELTA_ARRAY_SIZE];

    for (int i = 0; i < ROTARY_DELTA_ARRAY_SIZE; i++) {
        data[i] = temp[i];
    }

    return data;
}

#endif // !ROTARY_HANDLER_H