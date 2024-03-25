#include <SPI.h>
#include <MFRC522.h>
#include "display_controls.h"

// PINS
#define RST_PIN 9 // Configurable, see typical pin layout above
#define SS_PIN 10 // Configurable, see typical pin layout above
#define buzzer_pin 8
#define redLed_pin 5
#define greenLed_pin 6
#define DS1_pin 2
#define STCP1_pin 4
#define SHCP1_pin 3
#define relay_pin 7

// CONFIGS
#define seconds_open 3
#define buzzerDelay 500

MFRC522 mfrc522(SS_PIN, RST_PIN); // Create MFRC522 instance
byte accessUID[4] = {0x33, 0x7D, 0x21, 0xF8};

void open_door()
{
  digitalWrite(greenLed_pin, HIGH);
  digitalWrite(relay_pin, LOW);

  unsigned long prevTime1 = millis();
  unsigned long prevTime2 = millis();
  bool toneTurn = false;
  int seconds_left = seconds_open;

  DisplayDigit(seconds_left, DS1_pin, STCP1_pin, SHCP1_pin);
  tone(buzzer_pin, 1000);
  seconds_left--;

  while (seconds_left >= 0)
  {
    unsigned long currentTime = millis();
    // display
    if (currentTime - prevTime1 > 1000)
    {
      DisplayDigit(seconds_left, DS1_pin, STCP1_pin, SHCP1_pin);
      prevTime1 = currentTime;
      seconds_left--;
    }
    // buzzer
    if (currentTime - prevTime2 > buzzerDelay)
    {
      if (toneTurn == false)
      {
        tone(buzzer_pin, 900);
        prevTime2 = currentTime;
        toneTurn = !toneTurn;
      }
      else
      {
        tone(buzzer_pin, 1000);
        prevTime2 = currentTime;
        toneTurn = !toneTurn;
      }
    }
  }
  DisplayDigit(10, DS1_pin, STCP1_pin, SHCP1_pin);
  digitalWrite(relay_pin, HIGH);
  noTone(buzzer_pin);
  digitalWrite(greenLed_pin, LOW);
}
void close_door()
{
  digitalWrite(redLed_pin, HIGH);
  tone(buzzer_pin, 600);
  delay(2000);
  noTone(buzzer_pin);
  digitalWrite(redLed_pin, LOW);
}

void setup()
{                     // Init SPI bus
  mfrc522.PCD_Init(); // Init MFRC522
  delay(4);           // Optional delay. Some board do need more time after init to be ready, see Readme

  pinMode(DS1_pin, OUTPUT);
  pinMode(STCP1_pin, OUTPUT);
  pinMode(SHCP1_pin, OUTPUT);
  pinMode(buzzer_pin, OUTPUT);
  pinMode(redLed_pin, OUTPUT);
  pinMode(greenLed_pin, OUTPUT);
  pinMode(relay_pin, OUTPUT);
  digitalWrite(relay_pin, HIGH);
}

void loop()
{
  // Reset the loop if no new card present on the sensor/reader. This saves the entire process when idle.
  if (!mfrc522.PICC_IsNewCardPresent())
  {
    return;
  }

  // Select one of the cards
  if (!mfrc522.PICC_ReadCardSerial())
  {
    return;
  }

  if (mfrc522.uid.uidByte[0] == accessUID[0] && mfrc522.uid.uidByte[1] == accessUID[1] && mfrc522.uid.uidByte[2] == accessUID[2] && mfrc522.uid.uidByte[3] == accessUID[3])
  {
    open_door();
  }
  else
  {
    close_door();
  }
}