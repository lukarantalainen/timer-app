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

Server::Server(int device) {
  int ret;
  listening_socket = ::socket(AF_UNIX, SOCK_STREAM, 0);
  if (listening_socket == -1) {
    std::perror("socket");
    std::exit(1);
  }

  std::memset(&addr, 0, sizeof(addr));

  addr.sun_family = AF_UNIX;
  int n = std::snprintf(addr.sun_path, sizeof(addr.sun_path), "%s", SOCKET_NAME);

  if (n < 0 || static_cast<size_t>(n) >= sizeof(addr.sun_path)) {
    std::cout << "Path too long" << "\n";
    std::exit(1);
  }

  ::unlink(SOCKET_NAME);
  ret = ::bind(listening_socket, reinterpret_cast<const sockaddr*>(&addr), sizeof(addr));

  if (ret == -1) {
    std::perror("bind");
    exit(1);
  }
  ::chmod(SOCKET_NAME, 0666);
  
  ret = ::listen(listening_socket, 20);
  if (ret == -1) {
    std::perror("listen");
    exit(1);
  }

  std::cout << "Server started" << "\n";
  accept(listening_socket);

  m_input = new Input(device, [this](const KeyEvent& event) { onKeyEvent(event); });
  m_input->start();
}

Server::~Server() {
  delete m_input;
  ::close(listening_socket);
  ::close(data_socket);
  ::unlink(SOCKET_NAME);
}

void Server::accept(int socket) {
  ::close(data_socket);
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

SerializedKeyEvent serialize(const KeyEvent& event) {
  SerializedKeyEvent s;
  s.input_data = event.input_data;
  int n = std::snprintf(s.key_name, sizeof(s.key_name), "%s", event.key_name.c_str());
  return s;
}

int Server::onKeyEvent(const KeyEvent event) {
  if (!connected) {
    accept(listening_socket);
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

int main(int argc, char* argv[]) {
  std::signal(SIGPIPE, SIG_IGN);
  std::signal(SIGTERM, handleSignal);
  
  int device = argc > 1 ? argv[1][0] - '0' : 6;

  Server server(device);

  return 0;
}
