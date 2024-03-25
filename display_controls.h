#ifndef DISPLAY_CONTROLS_H
#define DISPLAY_CONTROLS_H
#include <Arduino.h>

void DisplayDigit(int Digit, int DS1_pin, int STCP1_pin, int SHCP1_pin);
void setup_modes(int DS1_pin, int STCP1_pin, int SHCP1_pin);

#endif