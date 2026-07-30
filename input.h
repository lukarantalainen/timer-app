#pragma once

#include <linux/input.h>

#include <QObject>
#include <QString>
#include <functional>
#include <string>
#include <thread>

struct KeyEvent {
  input_event input_data;
  std::string key_name;
};

class Input : public QObject {
  Q_OBJECT

 public:
  Input(int device) : device{device} {}
  void start();

 signals:
  void keyDown(const KeyEvent);
  void keyUp(const KeyEvent);

 private:
  int log();

  int device;
  std::thread* m_thread;
};

int listInputs();
