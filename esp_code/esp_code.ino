#include <ESP8266WiFi.h>
#include <ESP8266WiFiMulti.h>
#include <ESP8266HTTPClient.h>
#include <WiFiClient.h>
#include <LittleFS.h>

ESP8266WiFiMulti WiFiMulti;

#define SSID "[ONE]_HUNDRED"
#define PASSWORD "areyougigabyte"
#define SERVER_IP "192.168.31.253"
#define DATABASE_SIZE 100

uint8_t database[DATABASE_SIZE][4];
boolean is_download_successful = false;

void write_keys_to_db(String str_payload){
  int payload_length = str_payload.length()+1;
  char payload[payload_length];
  str_payload.toCharArray(payload, payload_length);
  
  int keys_start = -1;
  for (int i = 0; i < payload_length - 3; i++) {
      if (strncmp(&payload[i], "keys", 4) == 0) {
          keys_start = i + 8;
          break;
      }
  }

  if (keys_start == -1) {
      return;
  }
  
  uint8_t num = 0;
  uint8_t num_turn = 0;
  uint8_t key_turn = 0;
  
  for (int i = keys_start; i < payload_length; i++) {
    if (payload[i] == ',') {
      database[key_turn][num_turn] = num;
      num_turn++;
      num = 0;
    } else if (payload[i] == ']') {
      database[key_turn][num_turn] = num;
      if (payload[i+1] == ']'){
        break;
      }
      i+=2;
      num_turn=0;
      key_turn++;
      num = 0;
    } else {
      num = num * 10 + (payload[i] - '0');
    }
  }
  return;
}

int is_equal_arrays(uint8_t array1[], uint8_t array2[]) {
  for (uint8_t i = 0; i < 4; i++) {
    if (array1[i] != array2[i]) {
      return 0;
    }
  }
  return 1;
}

int is_key_in_database(uint8_t key[]) {
  for (uint8_t i = 0; i < DATABASE_SIZE; i++) {
    if (is_equal_arrays(key, database[i]) == 1) {
      return 1;
    }
  }
  return 0;
}

void setup() {
  Serial.begin(9600);
  for (uint8_t t = 4; t > 0; t--) {
    delay(1000);
  }
  WiFi.mode(WIFI_STA);
  WiFiMulti.addAP(SSID, PASSWORD);

  delay(2000);

  if (!LittleFS.begin()) {
    return;
  }

  for (uint8_t i=0; i<10;i++){
    if ((WiFiMulti.run() == WL_CONNECTED)) {
      download_keys();
      break;
    }
    delay(5000);
  }
  if(is_download_successful == false){
    readFile("/keys.txt");
  }
  LittleFS.end();
  Serial.write(2);
}

void loop() {
  delay(10);

  if (Serial.available() < 4) {
    return;
  }
  
  uint8_t rfid_code[4];

  for(uint8_t i=0;i<4;i++){
    rfid_code[i] = Serial.read();
  }

  if (is_key_in_database(rfid_code)==1){
    Serial.write(1);
    send_key(rfid_code);
  } else {
    Serial.write(0);
  }
}
