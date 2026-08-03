#pragma once

#include <linux/input.h>

#include <string>

#include "connection.h"

struct KeyEvent {
  input_event input_data;
  std::string key_name;
};

struct SerializedKeyEvent {
  input_event input_data;
  char key_name[MAX_KEY_SIZE];
};
