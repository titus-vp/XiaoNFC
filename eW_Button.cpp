#include "eW_Button.h"


uint8_t lastButtonState = HIGH;
unsigned long lastDebounceTime = 0;
unsigned long debounceDelay = 50;



void buttonInit() {
  pinMode(BUTTONPIN, INPUT);
}

bool buttonIsPressed() {
  bool currentButtonState = digitalRead(BUTTONPIN);
  static bool buttonPressed = false;  // Neue Variable für den Tastendruck-Status

  // Wenn der Schalter aufgrund von Rauschen oder Drücken geändert wurde:
  if (currentButtonState != lastButtonState) {
    // Setze den Debouncing-Timer zurück
    lastDebounceTime = millis();
  }

  // Überprüfe, ob der letzte Zustand des Schalters über den Zeitraum debounceDelay konstant war
  // Wenn ja und der Zustand niedrig ist, wurde der Button gedrückt
  if (((millis() - lastDebounceTime) > debounceDelay) && (currentButtonState == LOW) && !buttonPressed) {
    Serial.println("Button has been pressed");
    buttonPressed = true;  // Setze den Tastendruck-Status
    return true;
  } else if (currentButtonState == HIGH) {
    // Wenn der Taster losgelassen wurde, setze den Tastendruck-Status zurück
    buttonPressed = false;
  }

  // Aktualisiere den letzten Schalterzustand für die Entprellbedingung
  lastButtonState = currentButtonState;
  return false;
}
