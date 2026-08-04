#include "client.h"

#include <linux/input.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <unistd.h>

#include <QApplication>
#include <csignal>
#include <cstdlib>
#include <cstring>
#include <iostream>

#include "connection.h"
#include "keyevent.h"

Client::Client() {
  int ret;

  data_socket = socket(AF_UNIX, SOCK_STREAM, 0);
  if (data_socket == -1) {
    std::perror("socket");
  }

  std::memset(&addr, 0, sizeof(addr));

  addr.sun_family = AF_UNIX;
  std::strncpy(addr.sun_path, SOCKET_NAME, sizeof(addr.sun_path) - 1);
  addr.sun_path[sizeof(addr.sun_path) - 1] = '\0';

  // ret = ::connect(data_socket, (const sockaddr*)&addr, sizeof(addr));

  // if (ret == -1) {
  //   fprintf(stderr, "The server is down.\n");
  // }
  connect();

  m_thread = std::thread(&Client::listen, this);
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
      return;
    }
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
  return rv;
}

void Client::listen() {
  while (true) {
    size_t size;
    recv_all(&size, sizeof(size));


    int n = recv_all(&buffer, size);

    if (n == 0) {
      std::cout << "Server has closed connection" << "\n";
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
