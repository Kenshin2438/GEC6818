#include "tcp_sender.h"

#include <arpa/inet.h>
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>

void send_msg(const char *data) {
  struct sockaddr_in server_addr;
  int sock;

  if ((sock = socket(AF_INET, SOCK_STREAM, 0)) == -1) {
    perror("socket");
    exit(EXIT_FAILURE);
  }

  server_addr.sin_family = AF_INET;
  server_addr.sin_port = htons(SERVER_PORT);
  server_addr.sin_addr.s_addr = inet_addr(SERVER_IP);

  if (connect(sock, (struct sockaddr *)&server_addr, sizeof(server_addr)) == -1) {
    perror("connection");
    exit(EXIT_FAILURE);
  }

  if (send(sock, data, strlen(data), 0) == -1) {
    perror("send");
    exit(EXIT_FAILURE);
  }

  close(sock);
}
