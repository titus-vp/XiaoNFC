#include "eW_Feedback.h"

bool feedbackInit()
{
  pinMode(GREENLED, OUTPUT);
  pinMode(BLUELED, OUTPUT);
  pinMode(VIBRATIONMOTOR, OUTPUT);
}

void gLedPWM(uint8_t dutyCycle)
{
  analogWrite(GREENLED, dutyCycle);
}

void gLedTurnOff()
{
  analogWrite(GREENLED, 0);
}

void bLedPWM(uint8_t dutyCycle)
{
  analogWrite(BLUELED, dutyCycle);
}

void bLedTurnOff()
{
  analogWrite(BLUELED, 0);
}

void vibrationPWM(uint8_t dutyCycle)
{
  analogWrite(VIBRATIONMOTOR, dutyCycle);
}

void vibrationTurnOff()
{
  analogWrite(VIBRATIONMOTOR, 0);
}