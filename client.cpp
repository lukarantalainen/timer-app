#include "connection.h"
#include <sys/socket.h>
#include <sys/un.h>

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <linux/input.h>
#include <iostream>
#include "input.h"

struct SerializedKeyEvent {
  input_event input_data;
  char key_name[32];
};

KeyEvent deserialize(const char* buffer) {
  KeyEvent event;

  const char* p = buffer;
  
  memcpy(&event.input_data, p, sizeof(input_event));
  p += sizeof(input_event);

  char str_buff[32];
  strcpy(str_buff, p);
  event.key_name = std::string(str_buff);

  return event;
}

int main(int argc, char** argv) {
  int ret;
  int data_socket;
  sockaddr_un addr;
  char buffer[BUFFER_SIZE];

  data_socket = socket(AF_UNIX, SOCK_SEQPACKET, 0);
  if (data_socket == -1) {
    perror("socket");
    exit(EXIT_FAILURE);
  }

  memset(&addr, 0, sizeof(addr));

  addr.sun_family = AF_UNIX;
  strncpy(addr.sun_path, SOCKET_NAME, sizeof(addr.sun_path) - 1);

  ret = connect(data_socket, (const sockaddr *) &addr, sizeof(addr));
  
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
      std::cout << event.key_name << "\n";
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
