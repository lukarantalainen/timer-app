#pragma once

#include <linux/input.h>

#include <functional>
#include <string>
#include <thread>
#include "keyevent.h"

class Server;

class Input {

 public:
  Input(int device, Server* server);

  void keyDown(const KeyEvent);
  void keyUp(const KeyEvent);
  int log();

 private:

  int device;
  std::thread* m_thread;
  Server* m_server;
};

int listInputs();
