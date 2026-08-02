#include "connection.h"
#include <sys/socket.h>
#include <sys/un.h>

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>


int main(int argc, char** argv) {
  int ret;
  int data_socket;
  ssize_t r, w;
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

  strcpy(buffer, "END");
  w = write(data_socket, buffer, strlen(buffer) + 1);
  if (w == -1) {
    perror("write");
    exit(EXIT_FAILURE);
  }

  if (r == -1) {
    perror("read");
    exit(EXIT_FAILURE);
  }

  while (true) {
    ssize_t n = recv(data_socket, buffer, sizeof(buffer), 0);

    if (n > 0) {
      buffer[sizeof(buffer) - 1] = 0;
      printf("%s\n", buffer);
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
