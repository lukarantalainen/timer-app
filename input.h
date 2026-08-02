#pragma once

#include <linux/input.h>

#include <functional>
#include <string>
#include <thread>

#include "server.h"

struct KeyEvent {
  input_event input_data;
  std::string key_name;
};

class Input {

 public:
  Input(int device);

  void keyDown(const KeyEvent);
  void keyUp(const KeyEvent);
  int log();

 private:

  int device;
  std::thread* m_thread;
  Server* m_server;
};

int listInputs();
