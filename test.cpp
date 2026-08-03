#include <linux/input.h>

#include <functional>
#include <string>
#include <thread>

#include "connection.h"
#include <sys/socket.h>
#include <sys/un.h>

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <linux/input.h>
#include "input.h"
#include <iostream>
#include <cstring>

struct SerializedKeyEvent {
  input_event input_data;
  char key_name[32];
};

SerializedKeyEvent serialize(KeyEvent event) {
  SerializedKeyEvent s;
  s.input_data = event.input_data;
  strncpy(s.key_name, event.key_name.c_str(), sizeof(s.key_name) - 1);
  return s;
}

KeyEvent deserialize(const char* buffer) {
  KeyEvent event;

  const char* p = buffer;
  
  memcpy(&event.input_data, p, sizeof(input_event));
  p += sizeof(input_event);

  char str_buff[32];
  strcpy(str_buff, p);
  event.key_name = std::string(str_buff);
  
  return event;
}

int main() {
  input_event ev;
  ev.code = 69;
  ev.type = 1;
  ev.value = 324;
  KeyEvent event{
    ev, "hello"
  };
  auto s = serialize(event);
  const char* bytes = reinterpret_cast<const char*>(&s);

  KeyEvent des = deserialize(bytes);
  printf("%d %d %d %s\n", des.input_data.code, des.input_data.type, des.input_data.value, des.key_name.c_str());

  const char* str = "abcdefghijklmnopqrstuvwxyz";

  char test_str[16];
  std::strncpy(test_str, str, sizeof(test_str) - 1);
  test_str[sizeof(test_str) - 1] = '\0';
  std::cout << test_str << "\n";

  return 0;
}
