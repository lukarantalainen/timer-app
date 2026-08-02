#pragma once

#include <linux/input.h>

#include <functional>
#include <string>
#include <thread>

struct KeyEvent {
  input_event input_data;
  std::string key_name;
};

class Input {

 public:
  Input(int device) : device{device} {}
  void start();

  void keyDown(const KeyEvent);
  void keyUp(const KeyEvent);
  int log();

 private:

  int device;
  std::thread* m_thread;
};

int listInputs();
