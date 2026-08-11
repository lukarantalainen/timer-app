#include "client.h"

#include <linux/input.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <unistd.h>
#include <poll.h>

#include <QApplication>
#include <csignal>
#include <cstdlib>
#include <cstring>
#include <iostream>

#include "connection.h"
#include "keyevent.h"

Client::Client() { m_thread = std::thread(&Client::start, this); }

void Client::start() {
  std::memset(&addr, 0, sizeof(addr));

  addr.sun_family = AF_UNIX;
  std::snprintf(addr.sun_path, sizeof(addr.sun_path), "%s", SOCKET_NAME);

  connect();
  listen();
}

void Client::connect() {
  int ret;
  int count{1};
  ::close(data_socket);

  data_socket = socket(AF_UNIX, SOCK_STREAM, 0);
  if (data_socket == -1) {
    std::perror("socket");
  }

  while (true) {
    ret = ::connect(data_socket, (const sockaddr*)&addr, sizeof(addr));
    if (ret == 0) {
      std::cout << "Connected" << "\n";
      emit connectionChanged(true);
      return;
    }
    emit connectionChanged(false);
    std::this_thread::sleep_for(std::chrono::seconds(1));
    std::cout << "Trying to connect: " << count << "\n";
    ++count;
  }
}

void Client::onKeyEvent(const KeyEvent& event) {
  if (event.input_data.value == 0) {
    emit(keyUp(event));
  } else if (event.input_data.value == 1 || event.input_data.value == 2) {
    emit(keyDown(event));
  }
}

int Client::recv_all(void* data, size_t size) {
  char* ptr = static_cast<char*>(data);

  int rv = 0;

  while (rv < size) {
    int n = ::recv(data_socket, ptr, size - rv, 0);

    if (n == -1) {
      perror("recv");
      return -1;
    }
    rv += n;
  }

  char response = 'A';
  ::send(data_socket, &response, 1, 0);

  return rv;
}

void Client::listen() {
  while (true) {
    pollfd fd;
    fd.fd = data_socket;
    fd.events = POLLIN;
    int ret = ::poll(&fd, 1, 5000);

    if (ret <= 0) {
      emit connectionChanged(false);
    }

    size_t size;
    int n = recv_all(&size, sizeof(size));

    if (n == 0) {
      std::cout << "Server has closed connection" << "\n";
      emit connectionChanged(false);
      connect();
    } else if (n < 0) {
      std::perror("recv");
      connect();
    }

    n = recv_all(&buffer, size);

    if (n == 0) {
      std::cout << "Server has closed connection" << "\n";
      emit connectionChanged(false);
      connect();
    } else if (n < 0) {
      std::perror("recv");
      connect();
    }

    buffer[sizeof(buffer)] = '\0';
    KeyEvent event = deserialize(buffer);
    onKeyEvent(event);
  }
}

Client::~Client() { ::close(data_socket); }

bool isConnectionAlive(int sockfd) {
  return true;
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
