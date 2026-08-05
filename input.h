#pragma once

#include <linux/input.h>

#include <functional>
#include <string>
#include <thread>

#include "keyevent.h"

class Server;

class Input {
 public:
  Input(int device, std::function<void(const KeyEvent&)> callback);
  ~Input();

  void keyDown(const KeyEvent);
  void keyUp(const KeyEvent);
  int start();

 private:
  int m_device = -1;
  std::function<void(const KeyEvent&)> m_callback = nullptr;
};

int listInputs();
