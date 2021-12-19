/**
 * @file server.h
 * @author Jack
 * @mail chengjunjie.jack@gmail.com
 * @date 2021-12-16
 * @version 0.1
 *
 * @copyright Copyright (c) 2021
 */

#include <arpa/inet.h>
#include <errno.h>
#include <fcntl.h>
#include <netinet/in.h>
#include <pthread.h>
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>
#include <sys/socket.h>
#include <sys/stat.h>
#include <unistd.h>

#define SERVER_PORT 7087
#define BUFFSIZE 8192
#define IS_OK 0
#define IS_ERROR -1
#define LF (u_char)'\n'
#define CR (u_char)'\r'
#define CRLF "\r\n"
#define SP 0x20
#define URI_LIMIT 1024

char log_buffer[8192];

#define err_sys(str)                                        \
  {                                                         \
    __typeof__(errno) save_errno = errno;                   \
    fprintf(stderr, "%s: %s\n", str, strerror(save_errno)); \
    exit(-1);                                               \
  }
#define is_valid(c)                      \
  ({                                     \
    unsigned char _c = (unsigned char)c; \
    (_c >= 33) && (_c <= 126);           \
  })

void log_print(const char *format, ...) {
#ifndef DEBUG
  return;
#endif
  va_list arg_list;
  va_start(arg_list, format);
  vfprintf(stderr, format, arg_list);
}

enum http_method { GET = 1, POST, OTHER };
struct http_version {
  int major;
  int minor;
};
struct req_line {
  enum http_method method;
  char req_uri[URI_LIMIT];
  struct http_version version;
};

void server(u_int16_t);
void client_handler(int);
void *accept_request(void *);
int read_request_line(int, struct req_line *, char *, char *);
void close_request(int);
void response(int, int, const char *, char *, const void *, size_t);
void parse_method(enum http_method *, const char *, size_t n);
void parse_version(struct http_version *, char *, size_t n);
void req_parser(int, struct req_line *, char *);
// Send clntSock through sock.
// sock is a UNIX domain socket.
static void sendConnection(int clntSock, int sock) {
  struct msghdr msg;
  struct iovec iov[1];

  union {
    struct cmsghdr cm;
    char control[CMSG_SPACE(sizeof(int))];
  } ctrl_un = {0};
  struct cmsghdr *cmptr;

  msg.msg_control = ctrl_un.control;
  msg.msg_controllen = sizeof(ctrl_un.control);

  cmptr = CMSG_FIRSTHDR(&msg);
  cmptr->cmsg_len = CMSG_LEN(sizeof(int));
  cmptr->cmsg_level = SOL_SOCKET;
  cmptr->cmsg_type = SCM_RIGHTS;
  *((int *)CMSG_DATA(cmptr)) = clntSock;

  msg.msg_name = NULL;
  msg.msg_namelen = 0;

  iov[0].iov_base = "FD";
  iov[0].iov_len = 2;
  msg.msg_iov = iov;
  msg.msg_iovlen = 1;

  if (sendmsg(sock, &msg, 0) != 2)
    err_sys("Failed to send connection to child");
}
// Returns an open file descriptor received through sock.
// sock is a UNIX domain socket.
static int recvConnection(int sock) {
  struct msghdr msg;
  struct iovec iov[1];
  ssize_t n;
  char buf[64];

  union {
    struct cmsghdr cm;
    char control[CMSG_SPACE(sizeof(int))];
  } ctrl_un;
  struct cmsghdr *cmptr;

  msg.msg_control = ctrl_un.control;
  msg.msg_controllen = sizeof(ctrl_un.control);

  msg.msg_name = NULL;
  msg.msg_namelen = 0;

  iov[0].iov_base = buf;
  iov[0].iov_len = sizeof(buf);
  msg.msg_iov = iov;
  msg.msg_iovlen = 1;

  for (;;) {
    n = recvmsg(sock, &msg, 0);
    if (n == -1) {
      if (errno == EINTR) continue;
      die("Error in recvmsg");
    }
    // Messages with client connections are always sent with
    // "FD" as the message. Silently skip unsupported messages.
    if (n != 2 || buf[0] != 'F' || buf[1] != 'D') continue;

    if ((cmptr = CMSG_FIRSTHDR(&msg)) != NULL &&
        cmptr->cmsg_len == CMSG_LEN(sizeof(int)) &&
        cmptr->cmsg_level == SOL_SOCKET && cmptr->cmsg_type == SCM_RIGHTS)
      return *((int *)CMSG_DATA(cmptr));
  }
}