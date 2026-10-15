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
 ~Client();
 
 void start();
 void stop();

 signals:
  void keyDown(const KeyEvent& event);
  void keyUp(const KeyEvent& event);
  void connectionChanged(bool connected);
  void connectionCountdown(double value);

 private:
  int data_socket = -1;
  sockaddr_un addr;
  std::thread m_thread;

  void initiate();
  void listen();
  int connect();
  void onKeyEvent(const KeyEvent& event);
  int recv_all(void* data, size_t size);
};

KeyEvent deserialize(const char* buffer);
