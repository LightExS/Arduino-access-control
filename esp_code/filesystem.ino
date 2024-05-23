void readFile(const char *path) {
  File file = LittleFS.open(path, "r");
  if (!file) {
    return;
  }

  String payload;
  char byte_read;
  while (file.available()) { 
    byte_read = file.read();
    payload += byte_read;
    }
  write_keys_to_db(payload);

  file.close();
}

void writeFile(const char *path, String message) {
  File file = LittleFS.open(path, "w+");
  if (!file) {
    return;
  }
  int str_length = message.length()+1;
  char to_write[str_length];
  message.toCharArray(to_write, str_length);

  file.print(message);

  delay(2000);  // Make sure the CREATE and LASTWRITE times are different
  file.close();
}