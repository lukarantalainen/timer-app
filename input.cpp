#include "input.h"

#include <dirent.h>
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
#include <functional>
#include <iostream>
#include <map>
#include <sstream>
#include <string>
#include <vector>

#include "table.h"

#include <sys/socket.h>
#include <sys/un.h>

#include "server.h"

constexpr char DEV_PATH[] = "/dev/input/event";

const char* codename(unsigned int type, unsigned int code) {
  return (type <= EV_MAX && code <= max_size[type] && types[type] && types[type][code]) ? types[type][code] : "?";
}

Input::Input(int device, Server* server) : device{device}, m_server{server} {
  m_thread = new std::thread(&Input::log, this);
}

int listInputs() {
  DIR* dir = opendir("/dev/input");
  if (!dir) {
    perror("Failed to open directory");
    return 1;
  }

  struct dirent* entry = readdir(dir);

  std::vector<std::string> paths;
  while (entry != nullptr) {
    std::string name(entry->d_name);
    if (name.find("event") != std::string::npos) {
      std::string path = "/dev/input/" + name;
      paths.push_back(path);
    }
    entry = readdir(dir);
  }

  for (auto path : paths) std::cout << path << "\n";

  closedir(dir);
  return 0;
}

void Input::keyUp(KeyEvent){}
void Input::keyDown(KeyEvent){}

int Input::log() {
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
    ret = poll(&fd, 1, -1);

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

      KeyEvent event{*input_data, key};
      m_server->onKeyPress();
      if (input_data->value) {
        keyDown(event);
      } else {
        keyUp(event);
      }

      memset(input_data, 0, input_size);
    }
  }

  close(fd.fd);
  delete input_data;

  return 0;
}
volatile sig_atomic_t signaled = 0;

void handleSignal(int signal) {
  signaled = 1;
}

int main() {
  using namespace std::chrono_literals;

  std::signal(SIGTERM, handleSignal);

  Server server;
  Input input(6, &server);

  while(!signaled) {
    pause();
  }

  return 0;
}
