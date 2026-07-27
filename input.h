#pragma once 

#include <thread>
#include <functional>
#include <QString>
#include <QObject>

class Input : public QObject {

    Q_OBJECT

    public:
    Input(int device) : device{device} {}
    void start();
    
    signals:
        void keyPressed(const QString& key);

    private:
    int log();

    int device;
    std::thread* thread;
};
