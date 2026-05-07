#include <fcntl.h>
#include <stdio.h>
#include <unistd.h>
#include <poll.h>
#include <time.h>
#include <string.h>
#include <stdlib.h>

#include <linux/kernel.h>

constexpr char DEV_PATH[] = "/dev/input/event";

struct input_event {
    struct timeval time;
    unsigned short type;
    unsigned short code;
    int value;
};

int main(int argc, char* argv[])
{
    const int timeout = -1;
    char* input_dev = argv[1];

    char* path = new(char[strlen(DEV_PATH)+1+1]);

    strcpy(path, DEV_PATH);
    if (input_dev) {
        strcat(path, input_dev);
    } else {
        strcat(path, "0");
    }
    
    struct pollfd fds[1];
    
    fds[0].fd = open(path, O_RDONLY | O_NOCTTY | O_NONBLOCK);

    if (fds[0].fd < 0) {
        perror("failed to open device");
        //return (-1);
    }

    const int input_size = sizeof(input_event);
    struct input_event* input_data = new input_event;
    memset(input_data,0,input_size);

    fds[0].events = POLLIN;
    
    int ret;

    while(true) {
        ret = poll(fds, 1, timeout);
        
        if (ret < 0) {
            printf("Failed poll");
            break;
        }

        if (!fds[0].revents) {
            continue;
        }
        
        ssize_t r = read(fds[0].fd, input_data, input_size);

        if (r < 0) {
            printf("failed");
            break;
        } else {
            printf("time: %lu type: %hu code: %hu value: %d\n", input_data->time.tv_sec, input_data->type, input_data->code, input_data->value);
            memset(input_data,0,input_size);
        }
    }
    
    close(fds[0].fd);
    
    return 0;

}
