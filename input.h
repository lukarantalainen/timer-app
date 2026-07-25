#ifndef INPUT_H
#define INPUT_H

#include <thread>
#include <QLabel>
class Input {
    public:
    Input(int device, QLabel* keyboard_label) {
        auto thread = new std::thread(&Input::log, this, device, keyboard_label);
    }

    private:
    int log(int device, QLabel* keyboard_label = nullptr);
};

#endif
