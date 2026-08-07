#include "server.h"

#include <sys/socket.h>
#include <sys/stat.h>
#include <sys/un.h>
#include <unistd.h>

#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <csignal>
#include <iostream>
#include <print>

#include "connection.h"
#include "input.h"
#include "keyevent.h"

volatile sig_atomic_t signaled = 0;

Server::Server() {
  connection_socket = socket(AF_UNIX, SOCK_STREAM, 0);
  if (connection_socket == -1) {
    std::perror("socket");
    std::exit(1);
  }

  std::memset(&name, 0, sizeof(name));

  name.sun_family = AF_UNIX;
  int n = std::snprintf(name.sun_path, sizeof(name.sun_path), "%s", SOCKET_NAME);

  if (n < 0 || static_cast<size_t>(n) >= sizeof(name.sun_path)) {
    std::cout << "Path too long" << "\n";
    std::exit(1);
  }

  ::unlink(SOCKET_NAME);
  int ret = ::bind(connection_socket, reinterpret_cast<const sockaddr*>(&name), sizeof(name));

  if (ret == -1) {
    std::perror("bind");
    exit(1);
  }
  ::chmod(SOCKET_NAME, 0666);
  
  ret = ::listen(connection_socket, 20);
  if (ret == -1) {
    std::perror("listen");
    exit(1);
  }

  std::cout << "Server started" << "\n";
  accept(connection_socket);

  m_input = new Input(6, [this](const KeyEvent& event) { onKeyEvent(event); });
  start();
}

void Server::start() {
  m_input->start();
}

void Server::accept(int socket) {
  while (true) {
    std::cout << "Waiting for connection" << "\n";
    data_socket = ::accept(socket, NULL, NULL);
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
  delete m_input;
  close(connection_socket);
  close(data_socket);
  unlink(SOCKET_NAME);
}

SerializedKeyEvent serialize(const KeyEvent& event) {
  SerializedKeyEvent s;
  s.input_data = event.input_data;
  int n = std::snprintf(s.key_name, sizeof(s.key_name), "%s", event.key_name.c_str());
  return s;
}

int Server::send_all(const void* data, const size_t size) {
  const char* ptr = static_cast<const char*>(data);
  size_t sent = 0;

  while (sent < size) {
    ssize_t n = ::send(data_socket, ptr + sent, size - sent, 0);
    if (n <= 0) {
      return n;
    }

    sent += n;
  }
  return sent;
}

void Server::clientDisconnected() {
  std::cout << "Client disconnected" << "\n";
  ::close(data_socket);
  accept(connection_socket);
}

int Server::onKeyEvent(const KeyEvent event) {
  if (!connected) {
    clientDisconnected();
  } else {
    auto serialized = serialize(event);
    size_t size = sizeof(serialized);

    int w = send_all(&size, sizeof(size));
    if (w <= 0) {
      connected = false;
      return 0;
    }

    w = send_all(&serialized, sizeof(serialized));
    if (w <= 0) {
      connected = false;
      return 0;
    }
  }
  return 0;
}

void handleSignal(int signal) { signaled = 1; }

int main() {
  std::signal(SIGPIPE, SIG_IGN);
  std::signal(SIGTERM, handleSignal);
  
  Server server;

  return 0;
}
