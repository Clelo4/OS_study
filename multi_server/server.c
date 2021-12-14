/**
 * @file server.c
 * @author Jack
 * @mail chengjunjie.jack@gmail.com
 * @date 2021-12-13
 * @version 0.1
 *
 * @copyright Copyright (c) 2021
 */

#include <arpa/inet.h>
#include <errno.h>
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <strings.h>
#include <sys/socket.h>
#include <unistd.h>

#define SERVER_PORT 8080
#define err_sys(str)                                        \
  {                                                         \
    __typeof__(errno) save_errno = errno;                   \
    fprintf(stderr, "%s: %s\n", str, strerror(save_errno)); \
    exit(save_errno);                                       \
  }

#define BUFFSIZE 8192

char recv_buffer[BUFFSIZE];
char send_buffer[BUFFSIZE];

void server(u_int16_t);
void client_handler(int);
void accept_request(int);

void server(u_int16_t port) {
  int serverfd, connfd;
  int on = 1;
  socklen_t len;
  struct sockaddr_in serveraddr, clientaddr;
  serverfd = socket(AF_INET, SOCK_STREAM, 0);
  if (serverfd == -1) err_sys("call socket function error");

  bzero(&serveraddr, sizeof(serveraddr));
  serveraddr.sin_family = AF_INET;
  serveraddr.sin_addr.s_addr = htonl(INADDR_ANY);
  serveraddr.sin_port = htons(port);

  if (-1 == setsockopt(serverfd, SOL_SOCKET, SO_REUSEADDR, &on,
                       (socklen_t)sizeof(on)))
    err_sys("setsockopt SO_REUSEADDR error");

  if (-1 == bind(serverfd, (const struct sockaddr *)&serveraddr,
                 (socklen_t)sizeof(serveraddr)))
    err_sys("bind socket fd error");
  if (-1 == listen(serverfd, 0)) err_sys("listen socket error");

  while (1) {
    if ((connfd = accept(serverfd, (struct sockaddr *)&clientaddr,
                         (socklen_t *)&len)) < 0) {
      if (errno == EINTR) continue;
      err_sys("socket accept error");
    }
    accept_request(connfd);
  }
}

void accept_request(int connfd) {
  size_t recv_num;
  shutdown(connfd, SHUT_RDWR);
  // close(connfd);
  return;
again:
  while ((recv_num =
              recv(connfd, (void *)recv_buffer, sizeof(recv_buffer), 0)) > 0) {
    // process
    printf("%s", recv_buffer);
  }
  if (recv_num == -1 && errno == EINTR)
    goto again;
  else if (recv_num == -1) {
    err_sys("socket recv error");
  }
  // read end
  // write to use
  send_buffer[0] = 'A';
  send(connfd, (const void *)send_buffer, 1, 0);
  shutdown(connfd, SHUT_RDWR);
}

int main() {
  char a = '\r';
  printf("%d\n", +a);
  return 0;
  server(SERVER_PORT);
  return 0;
}