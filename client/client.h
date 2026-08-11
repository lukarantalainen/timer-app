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
  void connectionChanged(bool connected);
  void connectionCountdown(int value);

 private:
  int data_socket = -1;
  sockaddr_un addr;
  std::thread m_thread;

  void start();
  void listen();
  void connect();
  void onKeyEvent(const KeyEvent& event);
  int recv_all(void* data, size_t size);
};

KeyEvent deserialize(const char* buffer);
