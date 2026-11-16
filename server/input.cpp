#include "input.h"

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
#include <utility>
#include <vector>
#include <chrono>

#include "table.h"

volatile sig_atomic_t signaled = 0;

const char* codename(unsigned int type, unsigned int code) {
  return (type <= EV_MAX && code <= max_size[type] && types[type] &&
          types[type][code])
             ? types[type][code]
             : "?";
}

void handleSignal(int signal) { signaled = 1; }

Input::Input(int device, std::function<void(const KeyEvent&)> callback)
    : device{device}, callback{callback} {
    std::signal(SIGTERM, handleSignal);
}

Input::~Input() {}


void Input::keyUp(KeyEvent) {}
void Input::keyDown(KeyEvent) {}

int Input::start() {
  std::string path{"/dev/input/event"};

  path += (device + '0');

  struct pollfd fd;

  fd.fd = open(path.c_str(), O_RDONLY | O_NOCTTY | O_NONBLOCK);
  std::cerr << "Path: " << path << "\n";

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
    if (signaled) break;

    ret = poll(&fd, 1, 100);

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
      callback(event);
    }
  }

  ::close(fd.fd);

  delete input_data;

  return 0;
}
