#include "eW_Feedback.h"

bool feedbackInit()
{
  pinMode(GREENLED, OUTPUT);
  pinMode(BLUELED, OUTPUT);
  pinMode(VIBRATIONMOTOR, OUTPUT);
  digitalWrite(GREENLED, HIGH);
  digitalWrite(BLUELED, HIGH);
  digitalWrite(VIBRATIONMOTOR, LOW);
}

void gLedOn()
{
  digitalWrite(GREENLED, LOW);
}

void gLedOff()
{
  digitalWrite(GREENLED, HIGH);
}

void bLedOn()
{
  digitalWrite(BLUELED, LOW);
}

void bLedOff()
{
  digitalWrite(BLUELED, HIGH);
}

void vibrationOn()
{
  digitalWrite(VIBRATIONMOTOR, HIGH);
}

void vibrationOff()
{
  digitalWrite(VIBRATIONMOTOR, LOW);
}

// nrf52 adc has 14 resolution -> highest value: 16383
void gLedPWM(uint16_t dutyCycle)
{
  analogWrite(GREENLED, dutyCycle);
}

void gLedPWMOff()
{
  analogWrite(GREENLED, 0);
}

void bLedPWM(uint16_t dutyCycle)
{
  analogWrite(BLUELED, dutyCycle);
}

void bLedPWMOff()
{
  analogWrite(BLUELED, 0);
}

void vibrationPWM(uint16_t dutyCycle)
{
  analogWrite(VIBRATIONMOTOR, dutyCycle);
}

void vibrationPWMOff()
{
  analogWrite(VIBRATIONMOTOR, 0);
}