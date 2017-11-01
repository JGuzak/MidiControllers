// @author: Jordan Guzak
// hardware state declarations

#ifndef HARDWARESTATES_H
#define HARDWARESTATES_H

#include "RotaryHandler.h"
#include "ButtonHandler.h"

// **************************************
volatile static int *curData;

// shift handling
volatile static bool shift = false;
const static int shiftPin = 6;
volatile static int shiftEncoderState[][ROTARY_DELTA_ARRAY_SIZE] = {
    { 44, 45, 0, 0, 0, 2, 10 },
    { 44, 45, 0, 0, 0, 16, 1 }
};

// LEDs and page 
const static int pages = 4;
static int curPage = 1;
const static int buttonLEDPins[pages] = { 2, 3, 4, 5 };
const static int rotaryLEDPins[pages] = { 8, 9, 10, 11 };

// paged encoders
const static int pageButtonEncoders = 4;
volatile static int curPageEncoder;
volatile static int page1EncoderState[][ROTARY_VALUE_ARRAY_SIZE] = {
    { 46, 47, 0, 0, 0, 1, 10, 0 },
    { 48, 49, 0, 0, 0, 1, 11, 0 },
    { 50, 51, 0, 0, 0, 1, 12, 0 },
    { 52, 53, 0, 0, 0, 1, 13, 0 }
};
volatile static int page2EncoderState[][ROTARY_VALUE_ARRAY_SIZE] = {
    { 46, 47, 0, 0, 0, 1, 14, 0 },
    { 48, 49, 0, 0, 0, 1, 15, 0 },
    { 50, 51, 0, 0, 0, 1, 16, 0 },
    { 52, 53, 0, 0, 0, 1, 17, 0 }
};
volatile static int page3EncoderState[][ROTARY_VALUE_ARRAY_SIZE] = {
    { 46, 47, 0, 0, 0, 1, 18, 0 },
    { 48, 49, 0, 0, 0, 1, 19, 0 },
    { 50, 51, 0, 0, 0, 1, 20, 0 },
    { 52, 53, 0, 0, 0, 1, 21, 0 }
};
volatile static int page4EncoderState[][ROTARY_VALUE_ARRAY_SIZE] = {
    { 46, 47, 0, 0, 0, 1, 22, 0 },
    { 48, 49, 0, 0, 0, 1, 23, 0 },
    { 50, 51, 0, 0, 0, 1, 24, 0 },
    { 52, 53, 0, 0, 0, 1, 25, 0 }
};

// paged buttons
volatile static int curPageButton;
volatile static int page1ButtonState[][BUTTON_ARRAY_SIZE] = {
    { 40, 0, 0, 1, 26 },
    { 41, 0, 0, 1, 27 },
    { 42, 0, 0, 1, 28 },
    { 43, 0, 0, 1, 29 }
};
volatile static int page2ButtonState[][BUTTON_ARRAY_SIZE] = {
    { 40, 0, 0, 1, 30 },
    { 41, 0, 0, 1, 31 },
    { 42, 0, 0, 1, 32 },
    { 43, 0, 0, 1, 33 }
};
volatile static int page3ButtonState[][BUTTON_ARRAY_SIZE] = {
    { 40, 0, 0, 1, 34 },
    { 41, 0, 0, 1, 35 },
    { 42, 0, 0, 1, 36 },
    { 43, 0, 0, 1, 37 }
};
volatile static int page4ButtonState[][BUTTON_ARRAY_SIZE] = {
    { 40, 0, 0, 1, 38 },
    { 41, 0, 0, 1, 39 },
    { 42, 0, 0, 1, 40 },
    { 43, 0, 0, 1, 41 }
};

#endif // !HARDWARESTATES_H
