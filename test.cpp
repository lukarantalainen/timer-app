
#include <string>
#include <fstream>
#include <iostream>

#include <linux/kernel.h>

struct input_event {
    struct timeval time;
    unsigned short type;
    unsigned short code;
    int value;
};

int main(int argc, char **argv)
{
    const std::string path = "/dev/input/mouse0";
    std::ifstream input(path);

    std::string line;

    while(std::getline(input, line)) {
        std::cout << line << "\n";
        
    } 
    
    return 0;

}
