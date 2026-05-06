#include <iostream>
#include <stdio.h>
#include <string>
#include <string.h>

#include "input.h"
#include <linux/kernel.h>

constexpr char DEV_INPUT_EVENT[11] = "/dev/input";

struct input_event {
    struct timeval time;
    unsigned short type;
    unsigned short code;
    int value;
};

Input::Input() {
    
    

}

int main() {
    Input input;

    return 0;
}
