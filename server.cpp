#include "server.h"

#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <sys/stat.h>
#include <sys/un.h>
#include <unistd.h>

#include "connection.h"
#include "input.h"

Server::Server() {
  unlink(SOCKET_NAME);

  connection_socket = socket(AF_UNIX, SOCK_SEQPACKET, 0);
  if (connection_socket == -1) {
    perror("socket");
    exit(EXIT_FAILURE);
  }

  memset(&name, 0, sizeof(name));

  name.sun_family = AF_UNIX;
  strncpy(name.sun_path, SOCKET_NAME, sizeof(name.sun_path) - 1);

  ret = bind(connection_socket, (const sockaddr*)&name, sizeof(name));

  chmod(SOCKET_NAME, 0666);

  if (ret == -1) {
    perror("bind");
    exit(EXIT_FAILURE);
  }

  ret = listen(connection_socket, 20);
  data_socket = accept(connection_socket, NULL, NULL);
  if (data_socket == -1) {
    perror("accept");
    exit(EXIT_FAILURE);
  } else {
    connected = true;
  }
}

void Server::acceptConnection() {
  while (true) {
    data_socket = accept(connection_socket, NULL, NULL);
    if (data_socket == -1) {
      perror("accept");
    } else {
      connected = true;
      break;
    }
  }
}

Server::~Server() {
  close(connection_socket);
  close(data_socket);
  unlink(SOCKET_NAME);
}

SerializedKeyEvent serialize(const KeyEvent& event) {
  SerializedKeyEvent s;
  s.input_data = event.input_data;
  strncpy(s.key_name, event.key_name.c_str(), sizeof(s.key_name) - 1);
  s.key_name[sizeof(s.key_name) - 1] = '\0';
  return s;
}

int Server::onKeyEvent(const KeyEvent& event) {
  if (connected) {
    auto serialized = serialize(event);
    w = send(data_socket, &serialized, sizeof(serialized), 0);
    if (w == -1) {
      connected = false;
    }
  } else {
    close(data_socket);
    acceptConnection();
  }
  return 0;
}
