#ifndef EW_FEEDBACK_H
#define EW_FEEDBACK_H

#include "Arduino.h"

#define GREENLED 29
#define BLUELED 5
#define VIBRATIONMOTOR 3

bool feedbackInit();

void gLedPWM(uint8_t dutyCycle);
void gLedTurnOff();

void bLedPWM(uint8_t dutyCycle);
void bLedTurnOff();

void vibrationPWM(uint8_t dutyCycle);
void vibrationTurnOff();

#endif
