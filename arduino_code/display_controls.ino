int digits[12][8]{
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
    {0, 0, 0, 0, 0, 0, 0, 0},  // empty digit
    {1, 1, 1, 1, 1, 1, 1, 1}  // all active
};

void DisplayDigit(int Digit)
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
