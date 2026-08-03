#include "input.h"

#include <dirent.h>
#include <fcntl.h>
#include <linux/input-event-codes.h>
#include <linux/input.h>
#include <linux/kernel.h>
#include <poll.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <time.h>
#include <unistd.h>

#include <algorithm>
#include <csignal>
#include <cstdlib>
#include <cstring>
#include <fstream>
#include <functional>
#include <iostream>
#include <map>
#include <sstream>
#include <string>
#include <vector>

#include "server.h"
#include "table.h"

constexpr char DEV_PATH[] = "/dev/input/event";
volatile sig_atomic_t signaled = 0;

const char* codename(unsigned int type, unsigned int code) {
  return (type <= EV_MAX && code <= max_size[type] && types[type] &&
          types[type][code])
             ? types[type][code]
             : "?";
}

Input::Input(int device, Server* server) : device{device}, m_server{server} {
  log();
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

void Input::keyUp(KeyEvent) {}
void Input::keyDown(KeyEvent) {}

int Input::log() {
  char* input_dev;

  char* path = new (char[strlen(DEV_PATH) + 1 + 1]);

  std::strcpy(path, DEV_PATH);
  if (device) {
    path[std::strlen(DEV_PATH)] = device + '0';
  } else {
    std::strcat(path, "0");
  }

  struct pollfd fd;

  fd.fd = open(path, O_RDONLY | O_NOCTTY | O_NONBLOCK);

  if (fd.fd < 0) {
    std::perror("failed to open device");
    return (-1);
  }

  const int input_size = sizeof(input_event);
  struct input_event* input_data = new input_event;
  std::memset(input_data, 0, input_size);

  fd.events = POLLIN;

  int ret;

  while (true) {
    if (signaled) {
      break;
    }

    ret = poll(&fd, 1, -1);

    if (ret < 0) {
      std::cout << "Failed poll" << "\n";
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
      std::cout << "failed read" << "\n";
      return -1;
    } else {
      std::string key{codename(type, code)};
      const char* n = codename(type, code);

      const KeyEvent event{*input_data, key};
      m_server->onKeyEvent(event);

      std::memset(input_data, 0, input_size);
    }
  }

  ::close(fd.fd);
  delete input_data;

  return 0;
}


void handleSignal(int signal) { signaled = 1; }

int main() {
  using namespace std::chrono_literals;

  std::signal(SIGTERM, handleSignal);

  Server server;
  Input input(6, &server);

  return 0;
}
