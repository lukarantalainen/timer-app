#include "client.h"

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



Client::Client() {
  int ret;

  data_socket = socket(AF_UNIX, SOCK_SEQPACKET, 0);
  if (data_socket == -1) {
    perror("socket");
    exit(EXIT_FAILURE);
  }

  std::memset(&addr, 0, sizeof(addr));

  addr.sun_family = AF_UNIX;
  std::strncpy(addr.sun_path, SOCKET_NAME, sizeof(addr.sun_path) - 1);
  addr.sun_path[sizeof(addr.sun_path) - 1] = '\0';

  ret = ::connect(data_socket, (const sockaddr*)&addr, sizeof(addr));

  if (ret == -1) {
    fprintf(stderr, "The server is down.\n");
    exit(EXIT_FAILURE);
  }

  m_thread = std::thread(&Client::listen, this); 
}

void Client::onKeyEvent(const KeyEvent& event) {
  if (event.input_data.value == 0) {
    emit(keyUp(event));
  } else if (event.input_data.value == 1 || event.input_data.value == 2) {
    emit(keyDown(event));
  }
}

void Client::listen() {
  while (true) {
    ssize_t n = recv(data_socket, buffer, sizeof(buffer), 0);

    if (n > 0) {
      buffer[sizeof(buffer)] = 0;
      KeyEvent event = deserialize(buffer);
      onKeyEvent(event);
    } else if (n == 0) {
      printf("no data");
      break;
    } else {
      perror("recv");
      break;
    }
  }
}

Client::~Client() {
  close(data_socket);
}

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

