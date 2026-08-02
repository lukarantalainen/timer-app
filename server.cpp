#include "connection.h"

#include <sys/socket.h>
#include <sys/un.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main() {
  int down_flag = 0;
  int ret;
  int connection_socket;
  int data_socket;
  int result;
  ssize_t r, w;
  sockaddr_un name;
  char buffer[BUFFER_SIZE];

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

  if (ret == -1) {
    perror("bind");
    exit(EXIT_FAILURE);
  }

  ret = listen(connection_socket, 20);

  while (true) {

    data_socket = accept(connection_socket, NULL, NULL);
    if (data_socket == -1) {
      perror("accept");
      exit(EXIT_FAILURE);
    }
  
    result = 0;
    while (true) {
      r = read(data_socket, buffer, sizeof(buffer));
      if (r == -1) {
        perror("read");
        exit(EXIT_FAILURE);
      }

      buffer[sizeof(buffer) - 1] = 0;

      if (!strncmp(buffer, "DOWN", sizeof(buffer))) {
        down_flag = 1;
        continue;
      }

      if (!strncmp(buffer, "END", sizeof(buffer))) {
        break;
      }

      if (down_flag) {
        continue;
      }

      result += atoi(buffer);
    }

    sprintf(buffer, "%d", result);
    send(data_socket, buffer, sizeof(buffer), 0);
    //w = write(data_socket, buffer, sizeof(buffer));
    if (w == -1) {
      perror("write");
      exit(EXIT_FAILURE);
    }

    close(data_socket);

    if (down_flag) {
      break;
    }
  }

  close(connection_socket);

  unlink(SOCKET_NAME);

  exit(EXIT_SUCCESS);
}
