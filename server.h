#pragma once

#include <sys/socket.h>
#include <sys/un.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include "connection.h"

class KeyEvent;

class Server {
  public:
  Server();
  ~Server();
  int onKeyEvent(const KeyEvent& event);
  void acceptConnection();
  void check_status();
  int send_all(const void* data, size_t size);

  private:
  int ret;
  int connection_socket;
  int data_socket;
  size_t r, w;
  sockaddr_un name;
  char buffer[BUFFER_SIZE];
  bool connected;
};
