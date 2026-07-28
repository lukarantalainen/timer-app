#pragma once 

#include <thread>
#include <functional>
#include <QString>
#include <QObject>
#include <linux/input.h>
#include <string>

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
        void keyPressed(const KeyEvent);

    private:
    int log();

    int device;
    std::thread* thread;
};
