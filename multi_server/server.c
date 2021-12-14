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
#include <string.h>
#include <strings.h>
#include <sys/socket.h>
#include <unistd.h>

#define SERVER_PORT 8082
#define err_sys(str)                                        \
  {                                                         \
    __typeof__(errno) save_errno = errno;                   \
    fprintf(stderr, "%s: %s\n", str, strerror(save_errno)); \
    exit(save_errno);                                       \
  }
int is_valid(char c) {
  unsigned char _c = (unsigned char)c;
  return (_c >= 33) && (_c <= 126);
}

#define BUFFSIZE 8192
#define IS_OK 0
#define IS_ERROR -1
#define LF (u_char)'\n'
#define CR (u_char)'\r'
#define CRLF "\r\n"
#define SP 0x20
#define URI_LIMIT 1024
char recv_buffer[BUFFSIZE];
char send_buffer[BUFFSIZE];

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
void accept_request(int);
int read_request_line(int, struct req_line *);
void close_request(int);
void response(int connfd, int status_code, const char *reason_phrase,
              const void *, size_t);
void parse_method(enum http_method *method, const char *buff, size_t n);
void parse_version(struct http_version *, char *buff, size_t n);

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
  struct req_line *req_buf = calloc(0, sizeof(struct req_line));

  // Read Request-Line
  // Request-Line format = Method SP Request-URI SP HTTP-Version CRLF
  if (IS_ERROR == read_request_line(connfd, req_buf)) {
    return;
  }
  if (req_buf->method != GET) {
    response(connfd, 405, "Method Not Allowed", 0, 0);
    return;
  }
  if (req_buf->version.major == -1 || req_buf->version.major == -1) {
    return response(connfd, 505, "HTTP Version Not Supported", 0, 0);
    return;
  }
  const char *mesg = "Hello world!";
  response(connfd, 200, "Ok", mesg, strlen(mesg));
}

int read_request_line(int connfd, struct req_line *req_line) {
  unsigned char c = 0;
  int ret = 0;
  enum req_state { method = 0, url, version, done };
  int recv_num = 0;

  enum req_state state = method;
  for (;;) {
    if ((ret = recv(connfd, &c, 1, 0)) <= 0) {
      goto ERROR;
    }
    switch (state) {
      case method:
        /* code */
        if (is_valid(c)) {
          recv_buffer[recv_num++] = c;
        } else if (c == SP) {
          parse_method(&req_line->method, recv_buffer, recv_num);
          recv_num = 0;
          ++state;
        } else
          goto ERROR;
        break;
      case url:
        if (is_valid(c) && recv_num + 1 < URI_LIMIT) {
          req_line->req_uri[recv_num++] = c;
        } else if (c == SP) {
          req_line->req_uri[recv_num] = 0;
          recv_num = 0;
          ++state;
        } else
          goto ERROR;
        break;
      case version:
        if (is_valid(c)) {
          recv_buffer[recv_num++] = c;
        } else if (c == CR) {
          parse_version(&req_line->version, recv_buffer, recv_num);
          recv_num = 0;
          ++state;
        } else {
          goto ERROR;
        }
        break;
      case done:
        if (c == LF)
          goto DONE;
        else
          goto ERROR;
        break;
      default:
        goto ERROR;
        break;
    }
  }
DONE:
  return 0;
ERROR:
  fprintf(stderr, "state: %d\n", state);
  response(connfd, 400, "Bad request", 0, 0);
  return IS_ERROR;
}

void response(int connfd, int status_code, const char *reason_phrase,
              const void *message, size_t m_size) {
  int send_num = 0;
  send_num = snprintf(send_buffer, sizeof(send_buffer), "HTTP/1.1 %d %s%s",
                      status_code, reason_phrase, CRLF);
  send(connfd, send_buffer, send_num, 0);
  // send headers
  send_num = snprintf(send_buffer, sizeof(send_buffer),
                      "Content-Type: text/html; charset=utf-8%s", CRLF);
  send(connfd, send_buffer, send_num, 0);
  send_num = snprintf(send_buffer, sizeof(send_buffer), "Content-Length:%ld%s",
                      m_size, CRLF);
  send(connfd, send_buffer, send_num, 0);

  // 分割换行
  send_num = snprintf(send_buffer, sizeof(send_buffer), "%s", CRLF);
  send(connfd, send_buffer, send_num, 0);

  // send body
  send_num = snprintf(send_buffer, sizeof(send_buffer), "%s%s",
                      (const char *)message, CRLF);
  send(connfd, send_buffer, send_num, 0);
  // end
  send_num = snprintf(send_buffer, sizeof(send_buffer), "%s", CRLF);
  send(connfd, send_buffer, send_num, 0);
  shutdown(connfd, SHUT_RDWR);
}

void parse_method(enum http_method *method, const char *buff, size_t n) {
  unsigned char first_char = buff[0];
  switch (first_char) {
    case 'G':
      if (strncasecmp("GET", (const char *)buff, 3) == 0) {
        *method = GET;
        return;
      }
      goto unsupport;
      break;
    case 'P':
      if (strncasecmp("POST", (const char *)buff, 3) == 0) {
        *method = POST;
        return;
      }
      goto unsupport;
      break;
    default:
      break;
  }
unsupport:
  *method = OTHER;
}
void parse_version(struct http_version *version, char *buff, size_t n) {
  int size = 0;
  int cnt = 0;
  int end = 0;
  // find /
  for (cnt = 0; cnt < n; ++cnt) {
    if (buff[cnt] == '/') {
      ++cnt;
      break;
    }
  }
  version->major = version->minor = -1;
  for (end = cnt; end < n; ++end) {
    // find major
    if (buff[end] == '.') {
      buff[end] = 0;
      version->major = strtol(buff + cnt, (char **)NULL, 10);
      buff[end] = '.';
      cnt = end + 1;
      break;
    }
  }
  if (cnt < n) {
    buff[n] = 0;
    if (version->major != -1) {
      version->minor = strtol(buff + cnt, (char **)NULL, 10);
    }
  }
}

int main() {
  // fprintf(stderr, "%d\n", strncasecmp("GET", "GET", 3));
  server(SERVER_PORT);
  return 0;
}

