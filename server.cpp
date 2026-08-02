#include "connection.h"
#include "server.h"

#include <sys/socket.h>
#include <sys/un.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

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

  if (ret == -1) {
    perror("bind");
    exit(EXIT_FAILURE);
  }

  ret = listen(connection_socket, 20);
  data_socket = accept(connection_socket, NULL, NULL);
  if (data_socket == -1) {
    perror("accept");
    exit(EXIT_FAILURE);
  }
  printf("client connected");
}

Server::~Server() {
  close(connection_socket);
  unlink(SOCKET_NAME);
}

int Server::onKeyPress() {
  w = send(data_socket, "hello\0", 6, 0);
  if (w == -1) {
    perror("send");
    exit(EXIT_FAILURE);
  }
  return 0;
}

// int main() {
//   // int ret;
//   // int connection_socket;
//   // int data_socket;
//   // ssize_t r, w;
//   // sockaddr_un name;
//   // char buffer[BUFFER_SIZE];
  
//   // unlink(SOCKET_NAME);

//   // connection_socket = socket(AF_UNIX, SOCK_SEQPACKET, 0);
//   // if (connection_socket == -1) {
//   //   perror("socket");
//   //   exit(EXIT_FAILURE);
//   // }

//   // memset(&name, 0, sizeof(name));

//   // name.sun_family = AF_UNIX;
//   // strncpy(name.sun_path, SOCKET_NAME, sizeof(name.sun_path) - 1);

//   // ret = bind(connection_socket, (const sockaddr*)&name, sizeof(name));

//   // if (ret == -1) {
//   //   perror("bind");
//   //   exit(EXIT_FAILURE);
//   // }

//   // ret = listen(connection_socket, 20);
//   // while (true) {
    

//   //   data_socket = accept(connection_socket, NULL, NULL);
//   //   if (data_socket == -1) {
//   //     perror("accept");
//   //     exit(EXIT_FAILURE);
//   //   }
//   //   printf("client connected");

//   //   while (true) {
//   //     sleep(1);
//   //     w = send(data_socket, "hello\0", 6, 0);
//   //     if (w == -1) {
//   //       perror("send");
//   //       exit(EXIT_FAILURE);
//   //     }
//   //   }
    
//   // }

//   // close(connection_socket);

//   // unlink(SOCKET_NAME);

//   Server server;
//   while (true) {
//     server.onKeyPress();
//     sleep(1);
//   }

//   exit(EXIT_SUCCESS);
// }
