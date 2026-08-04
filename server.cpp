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
  connection_socket = socket(AF_UNIX, SOCK_STREAM, 0);
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

void Server::check_status() {
  r = recv(data_socket, buffer, BUFFER_SIZE, 0);
  if (r == -1) {
    connected = true;
    std::cout << "Client disconnected";
    acceptConnection();
  }
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

int Server::send_all(const void* data, size_t size) {
  const char* ptr = static_cast<const char*>(data);
  size_t sent = 0;

  while (sent < size) {
    ssize_t n = ::send(data_socket, ptr + sent, size - sent, 0);
    if (n < 0) {
      return n;
    }
    sent += n;
  }
  return sent;
}

int Server::onKeyEvent(const KeyEvent& event) {
  if (!connected) {
    ::close(data_socket);
    acceptConnection();
  } else {
    auto serialized = serialize(event);
    size_t size = sizeof(serialized);

    send_all(&size, sizeof(size));

    w = send_all(&serialized, sizeof(serialized));
    if (w == -1) {
      std::cout << "Client disconnected" << "\n";
      connected = false;
    }
  }
  return 0;
}
