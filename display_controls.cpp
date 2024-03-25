#include "display_controls.h"

int digits[11][8]{
    {0, 1, 1, 1, 1, 1, 1, 0}, // digit 0
    {0, 0, 1, 1, 0, 0, 0, 0}, // digit 1
    {0, 1, 1, 0, 1, 1, 0, 1}, // digit 2
    {0, 1, 1, 1, 1, 0, 0, 1}, // digit 3
    {0, 0, 1, 1, 0, 0, 1, 1}, // digit 4
    {0, 1, 0, 1, 1, 0, 1, 1}, // digit 5
    {0, 1, 0, 1, 1, 1, 1, 1}, // digit 6
    {0, 1, 1, 1, 0, 0, 0, 0}, // digit 7
    {0, 1, 1, 1, 1, 1, 1, 1}, // digit 8
    {0, 1, 1, 1, 1, 0, 1, 1}, // digit 9
    {0, 0, 0, 0, 0, 0, 0, 0}  // empty digit
};

void DisplayDigit(int Digit, int DS1_pin, int STCP1_pin, int SHCP1_pin)
{
  digitalWrite(STCP1_pin, LOW);
  for (int i = 7; i >= 0; i--)
  {
    digitalWrite(SHCP1_pin, LOW);
    if (digits[Digit][i] == 0)
      digitalWrite(DS1_pin, LOW);
    if (digits[Digit][i] == 1)
      digitalWrite(DS1_pin, HIGH);
    digitalWrite(SHCP1_pin, HIGH);
  }
  digitalWrite(STCP1_pin, HIGH);
}

void setup_modes(int DS1_pin, int STCP1_pin, int SHCP1_pin){
  pinMode(DS1_pin, OUTPUT);
  pinMode(STCP1_pin, OUTPUT);
  pinMode(SHCP1_pin, OUTPUT);
  DisplayDigit(10, DS1_pin, STCP1_pin, SHCP1_pin);
}

void start_countdown(int DS1_pin, int STCP1_pin, int SHCP1_pin, int seconds){
    for (int i = seconds; i >= 0; i--)
    {
      DisplayDigit(i, DS1_pin, STCP1_pin, SHCP1_pin);
      delay(1000);
    }
    DisplayDigit(10, DS1_pin, STCP1_pin, SHCP1_pin);
}
