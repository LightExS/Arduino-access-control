void send_key(uint8_t key[4]){
  WiFiClient client;
  HTTPClient http;

  http.begin(client, "http://" SERVER_IP);
  http.addHeader("Content-Type", "application/json");
  String send_key;

  for (int i = 0; i < 3; i++){
    send_key += String(key[i])+", ";
  }
  send_key += String(key[3]);

  String payload = "{\"key\":["+send_key+"]}";
  int httpCode = http.POST(payload);
  http.end();
}

void download_keys(){
  HTTPClient http;
  WiFiClient client;

  http.begin(client, "http://" SERVER_IP);

  int httpCode = http.GET();

  // httpCode will be negative on error
  if (httpCode > 0) {
    // file found at server
    if (httpCode == HTTP_CODE_OK || httpCode == HTTP_CODE_MOVED_PERMANENTLY) {
      String payload = http.getString();

      write_keys_to_db(payload);
      writeFile("/keys.txt", payload);
      is_download_successful = true;
    }
  }
}