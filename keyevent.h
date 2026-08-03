#pragma once

#include <linux/input.h>
#include <string>

struct KeyEvent {
  input_event input_data;
  std::string key_name;
};
