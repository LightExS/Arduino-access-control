#include <SPI.h>
#include <MFRC522.h>
#include <SoftwareSerial.h>

// PINS
#define RST_PIN 9 // Configurable, see typical pin layout above
#define SS_PIN 10 // Configurable, see typical pin layout above
#define buzzer_pin 8
#define redLed_pin 5
#define greenLed_pin 6
#define DS1_pin A5
#define STCP1_pin 4
#define SHCP1_pin A4
#define relay_pin 7



// CONFIGS
#define seconds_open 5
#define buzzerDelay 500

MFRC522 mfrc522(SS_PIN, RST_PIN); // Create MFRC522 instance
SoftwareSerial mySerial(2,3);

void open_door()
{
  digitalWrite(greenLed_pin, HIGH);
  digitalWrite(relay_pin, LOW);

  unsigned long prevTime1 = millis();
  unsigned long prevTime2 = millis();
  bool toneTurn = false;
  int seconds_left = seconds_open;

  DisplayDigit(seconds_left);
  tone(buzzer_pin, 1000);
  seconds_left--;

  while (seconds_left >= 0)
  {
    unsigned long currentTime = millis();
    // display
    if (currentTime - prevTime1 > 1000)
    {
      DisplayDigit(seconds_left);
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
  DisplayDigit(10);
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

int check_valid_code(){
  int result;

  mySerial.write(mfrc522.uid.uidByte[0]);
  mySerial.write(mfrc522.uid.uidByte[1]);
  mySerial.write(mfrc522.uid.uidByte[2]);
  mySerial.write(mfrc522.uid.uidByte[3]);

  for(;;){
    if (mySerial.available() >= 1) {
      result = mySerial.read();
      break;
    }
  }
  return result;

}

void setup()
{ 
  SPI.begin(); 
  mfrc522.PCD_Init();

  Serial.begin(9600);
  mySerial.begin(9600);

  pinMode(DS1_pin, OUTPUT);
  pinMode(STCP1_pin, OUTPUT);
  pinMode(SHCP1_pin, OUTPUT);
  pinMode(buzzer_pin, OUTPUT);
  pinMode(redLed_pin, OUTPUT);
  pinMode(greenLed_pin, OUTPUT);
  pinMode(relay_pin, OUTPUT);
  digitalWrite(relay_pin, HIGH);

  DisplayDigit(11);

  while(1){
    if (mySerial.available() >= 1) {
      mySerial.read();
      break;
    }
  }

  DisplayDigit(10);
  Serial.println("Setup complete");
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

  
  if (check_valid_code()==1)
  {
    open_door();
  }
  else
  {
    close_door();
  }
}