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

#include "connection.h"

constexpr char DEV_PATH[] = "/dev/input/event";

const char* codename(unsigned int type, unsigned int code) {
  return (type <= EV_MAX && code <= max_size[type] && types[type] && types[type][code]) ? types[type][code] : "?";
}

void Input::start() { m_thread = new std::thread(&Input::log, this); }

void startConnection() {
  int down_flag = 0;
  int ret;
  int connection_socket;
  int data_socket;
  ssize_t r, w;
  struct sockaddr_un name;
  char buffer[BUFFER_SIZE];


  connection_socket = socket(AF_UNIX, SOCK_STREAM, 0);
  if (connection_socket == -1) {
    perror("socket");
    exit(EXIT_FAILURE);
  }

  memset(&name, 0, sizeof(name));

  name.sun_family = AF_UNIX;
  strncpy(name.sun_path, SOCKET_NAME, sizeof(name.sun_path) - 1);

  ret = bind(connection_socket, (const struct sockaddr *)&name, sizeof(name));
  if (ret == -1) {
    perror("bind");
    exit(EXIT_FAILURE);
  }

  ret = listen(connection_socket, 20);
  if (ret == -1) {
    perror("listen");
    exit(EXIT_FAILURE);
  }

  while (true) {
    r = read(data_socket, buffer, sizeof(buffer));
    if (r == -1) {
      perror("read");
      exit(EXIT_FAILURE);
    }

    buffer[sizeof(buffer)-1] = 0;

    if (!strncmp(buffer, "DOWN", sizeof(buffer))) {
      down_flag = 1;
      continue;
    }

    if (!strncmp(buffer, "END", sizeof(buffer))) {
      break;
    }

    if (down_flag) continue;
  }

  

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
      if (input_data->value) {
        std::cout << key << "\n";
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

int main() {
  std::cout << sizeof(KeyEvent) << "\n";
  Input input(6);
  input.log();

  std::cout << "test";

  return 0;
}
