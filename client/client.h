#pragma once

#include <sys/socket.h>
#include <sys/un.h>

#include <QObject>
#include <thread>

#include "connection.h"

#include "keyevent.h"

class Client : public QObject {
  Q_OBJECT
 public:
  Client();
  ~Client();
  

 signals:
  void keyDown(const KeyEvent& event);
  void keyUp(const KeyEvent& event);

 private:
  int data_socket;
  sockaddr_un addr;
  char buffer[BUFFER_SIZE];
  std::thread m_thread;

  void listen();
  void connect();
  void onKeyEvent(const KeyEvent& event);
  int recv_all(void* data, size_t size);
};

KeyEvent deserialize(const char* buffer);
