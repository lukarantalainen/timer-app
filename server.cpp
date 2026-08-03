#include "server.h"

#include <sys/socket.h>
#include <sys/stat.h>
#include <sys/un.h>
#include <unistd.h>

#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <iostream>

#include "connection.h"
#include "keyevent.h"

Server::Server() {
  connection_socket = socket(AF_UNIX, SOCK_SEQPACKET, 0);
  if (connection_socket == -1) {
    std::perror("socket");
    std::exit(1);
  }

  std::memset(&name, 0, sizeof(name));

  name.sun_family = AF_UNIX;
  std::strncpy(name.sun_path, SOCKET_NAME, sizeof(name.sun_path) - 1);
  name.sun_path[sizeof(name.sun_path) - 1] = '\0';

  ::unlink(SOCKET_NAME);
  ret = ::bind(connection_socket, (const sockaddr*)&name, sizeof(name));

  ::chmod(SOCKET_NAME, 0666);

  if (ret == -1) {
    std::perror("bind");
    exit(1);
  }

  ret = ::listen(connection_socket, 20);
  if (ret == -1) {
    std::perror("listen");
    exit(1);
  }

  std::cout << "Server started" << "\n";
  acceptConnection();
}

void Server::acceptConnection() {
  while (true) {
    data_socket = ::accept(connection_socket, NULL, NULL);
    if (data_socket == -1) {
      std::perror("accept");
    } else {
      std::cout << "Client connected" << "\n";
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
  std::strncpy(s.key_name, event.key_name.c_str(), sizeof(s.key_name) - 1);
  s.key_name[sizeof(s.key_name) - 1] = '\0';
  return s;
}

int Server::onKeyEvent(const KeyEvent& event) {
  if (connected) {
    auto serialized = serialize(event);
    w = ::send(data_socket, &serialized, sizeof(serialized), 0);
    if (w == -1) {
      std::cout << "Client disconnected" << "\n";
      connected = false;
    }
  } else {
    ::close(data_socket);
    acceptConnection();
  }
  return 0;
}
