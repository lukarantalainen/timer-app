#include "input.h"

#include <fcntl.h>
#include <linux/input-event-codes.h>
#include <linux/input.h>
#include <linux/kernel.h>
#include <poll.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>

#include <algorithm>
#include <csignal>
#include <fstream>
#include <iostream>
#include <map>
#include <sstream>
#include <string>
#include <vector>
#include <functional>

#include <QString>

#include "table.h"



int stop{};

constexpr char DEV_PATH[] = "/dev/input/event";

std::map<std::string, int> keys = {};

void interrupt_handler(int sig) { stop = true; }

const char* codename(unsigned int type, unsigned int code) {
  return (type <= EV_MAX) ? types[type][code] : "?";
}

void Input::start() {
    thread = new std::thread(&Input::log, this);
}


int Input::log() {
  //std::signal(SIGINT, interrupt_handler);
  //std::signal(SIGKILL, interrupt_handler);

  const int timeout = 5;
  char* input_dev;

  char* path = new (char[strlen(DEV_PATH) + 1 + 1]);

  strcpy(path, DEV_PATH);
  if (device) {
    path[strlen(DEV_PATH)] = device + '0';
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
  memset(input_data, 0, input_size);

  fd.events = POLLIN;

  int ret;

  while (true) {
    if (stop == 1) break;
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

    if (type != 1) continue;

    if (rd < input_size) {
      printf("failed\n");
      return -1;
    } else {
      std::string key{codename(type, code)};
      const char* n = codename(type, code);
      printf("%s\n", codename(type, code));
      if (keys.count(key)) {
        ++keys[key];
      } else {
        keys[key] = 1;
      }
        
        KeyEvent event {*input_data, key};
        emit keyPressed(event);
      memset(input_data, 0, input_size);
    }
  }

  close(fd.fd);
  delete input_data;

  return 0;
}
