/**
 * @file server.h
 * @author Jack
 * @mail chengjunjie.jack@gmail.com
 * @date 2021-12-16
 * @version 0.1
 *
 * @copyright Copyright (c) 2021
 */

/**
 * @file server.h
 * @author Jack
 * @mail chengjunjie.jack@gmail.com
 * @date 2021-12-15
 * @version 0.1
 *
 * @copyright Copyright (c) 2021
 */

#include <arpa/inet.h>
#include <errno.h>
#include <fcntl.h>
#include <netinet/in.h>
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>
#include <sys/socket.h>
#include <unistd.h>
#include <sys/stat.h>

#define SERVER_PORT 7087
#define BUFFSIZE 8192
#define IS_OK 0
#define IS_ERROR -1
#define LF (u_char)'\n'
#define CR (u_char)'\r'
#define CRLF "\r\n"
#define SP 0x20
#define URI_LIMIT 1024

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
void parse_uri(int, struct req_line *, char *);
