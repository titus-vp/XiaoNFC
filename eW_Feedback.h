// eW_Feedback.h

#ifndef EW_FEEDBACK_H
#define EW_FEEDBACK_H

#include "Arduino.h"

#define GREENLED 3
#define BLUELED 5
#define VIBRATIONMOTOR 1

bool feedbackInit();

void gLedOn();
void gLedOff();

void bLedOn();
void bLedOff();

void vibrationOn();
void vibrationOff();

// nrf52 adc has 14 resolution -> highest value: 16383
void gLedPWM(uint16_t dutyCycle);
void gLedPWMOff();

void bLedPWM(uint16_t dutyCycle);
void bLedPWMOff();

void vibrationPWM(uint16_t dutyCycle);
void vibrationPWMOff();

#endif
