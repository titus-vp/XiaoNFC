#ifndef EW_BUTTON_H
#define EW_BUTTON_H

#include "Arduino.h"

#define BUTTONPIN 4


extern uint8_t lastButtonState;     // the previous reading from the input pin

// the following variables are unsigned longs because the time, measured in
// milliseconds, will quickly become a bigger number than can be stored in an int.
extern unsigned long lastDebounceTime;  // the last time the output pin was toggled
extern unsigned long debounceDelay;     // the debounce time; increase if the output flickers



void buttonInit();
bool buttonIsPressed();

#endif
