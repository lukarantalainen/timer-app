#pragma once

#include <sys/socket.h>
#include <sys/un.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include "connection.h"
#include "input.h"

class KeyEvent;
class Server {
  public:
  Server();
  ~Server();
  int onKeyEvent(const KeyEvent event);
  void accept(int socket);
  int send_all(const void* data, const size_t size);
  void start();

  private:
  int connection_socket = -1;
  int data_socket = -1;
  sockaddr_un name;
  bool connected = false;

  Input* m_input = nullptr;

  void clientDisconnected();
};
