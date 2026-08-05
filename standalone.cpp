#include <linux/input.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <unistd.h>

#include <csignal>
#include <cstring>
#include <iostream>

#include "connection.h"
#include "input.h"

KeyEvent deserialize(const char* buffer) {
  KeyEvent event;

  const char* p = buffer;

  std::memcpy(&event.input_data, p, sizeof(input_event));
  p += sizeof(input_event);

  char str_buff[MAX_KEY_SIZE]{};
  std::strcpy(str_buff, p);
  event.key_name = std::string(str_buff);

  return event;
}

void handleInterrupt(int signal) {
  exit(EXIT_SUCCESS);
}

int main(int argc, char** argv) {
  std::signal(SIGINT, handleInterrupt);
  std::signal(SIGTERM, handleInterrupt);

  int ret;
  int data_socket;
  sockaddr_un addr;
  char buffer[BUFFER_SIZE];

  data_socket = socket(AF_UNIX, SOCK_SEQPACKET, 0);
  if (data_socket == -1) {
    perror("socket");
    exit(EXIT_FAILURE);
  }

  std::memset(&addr, 0, sizeof(addr));

  addr.sun_family = AF_UNIX;
  std::snprintf(addr.sun_path, sizeof(addr.sun_path), "%s", SOCKET_NAME);
  addr.sun_path[sizeof(addr.sun_path) - 1] = '\0';

  ret = connect(data_socket, (const sockaddr*)&addr, sizeof(addr));

  if (ret == -1) {
    fprintf(stderr, "The server is down.\n");
    exit(EXIT_FAILURE);
  }

  int count{};
  while (true) {
    ssize_t n = recv(data_socket, buffer, sizeof(buffer), 0);

    if (n > 0) {
      buffer[sizeof(buffer)] = 0;
      KeyEvent event = deserialize(buffer);
      std::cout << event.key_name << " value: " << event.input_data.value << " code: " << event.input_data.code << " type: " << event.input_data.type << "\n";
      ++count;
    } else if (n == 0) {
      printf("no data");
      break;
    } else {
      perror("recv");
      break;
    }
  }

  close(data_socket);
  exit(EXIT_SUCCESS);

  return 0;
}
