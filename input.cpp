#include <fcntl.h>
#include <stdio.h>
#include <unistd.h>
#include <poll.h>
#include <time.h>
#include <string.h>
#include <stdlib.h>

#include <iostream>
#include <fstream>
#include <csignal>

#include <linux/kernel.h>
#include <linux/input-event-codes.h>
#include <linux/input.h>

#include <map>
#include <string>
#include <vector>
#include <algorithm>

#include "table.h"

int stop{};

constexpr char DEV_PATH[] = "/dev/input/event";

std::map<std::string, int> keys = {};

void interrupt_handler(int sig) {
    switch(sig) {
        case 0: std::cout << "SIGTERM" << "\n";
        break;
        case 1: std::cout << "SIGSEGV" << "\n";
        break;
        case 2: std::cout << "\nSIGINT" << "\n";
        break;
        default: std::cout << "unknown signal: " << sig << "\n";
    }
    
    std::cout << signal << "\n";
	stop = true;
}

const char* codename(unsigned int type, unsigned int code)
{
	return (type <= EV_MAX) ? types[type][code] : "?";
}

int main(int argc, char* argv[])
{   
	std::signal(SIGINT, interrupt_handler);
    std::signal(SIGKILL, interrupt_handler);

    const int timeout = -1;
    char* input_dev = argv[1];

    char* path = new(char[strlen(DEV_PATH)+1+1]);

    strcpy(path, DEV_PATH);
    if (input_dev) {
        strcat(path, input_dev);
    } else {
        strcat(path, "0");
    }
    
    struct pollfd fd;
    
    fd.fd = open(path, O_RDONLY | O_NOCTTY | O_NONBLOCK);

    if (fd.fd < 0) {
        
        perror("failed to open device");
        return (-1);
    }

    const int input_size = sizeof(input_event);
    struct input_event* input_data = new input_event;
    memset(input_data,0,input_size);	

    fd.events = POLLIN;
    
    int ret;

    while(true) {
		if (stop==1) break;
        ret = poll(&fd, 1, timeout);
        
        if (ret < 0) {
            printf("Failed poll\n");
            break;
        }

        if (!fd.revents) {
            continue;
        }

        ssize_t rd = read(fd.fd, input_data, input_size);
        
        uint type, code;

        type = input_data->type;
        code = input_data->code;

		if (type!=1) continue;

        if (rd < input_size) {
            printf("failed\n");
            return -1;
        } else {
			std::string key{codename(type, code)};
			const char* n = codename(type, code);
            printf("%s\n", codename(type, code));
			if (keys.count(key)) {
				++keys[key];
			} else  {
				keys[key] = 1;
			}
            printf("time: %lu type: %hu code: %hu (%s) value: %d\n", input_data->time.tv_sec, type, code, n, input_data->value);
            memset(input_data,0,input_size);
        }
    }

	for (auto k : keys) {
		std::cout << k.first << ": " << k.second << "\n";
	}

	
	close(fd.fd);
    delete input_data;
    
    return 0;

}
