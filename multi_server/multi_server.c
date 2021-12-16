/**
 * @file multi_server.c
 * @author Jack
 * @mail chengjunjie.jack@gmail.com
 * @date 2021-12-16
 * @version 0.1
 *
 * @copyright Copyright (c) 2021
 */
/**
 * @file server.c
 * @author Jack
 * @mail chengjunjie.jack@gmail.com
 * @date 2021-12-13
 * @version 0.1
 *
 * @copyright Copyright (c) 2021
 */

#include <signal.h>
#include <sys/ipc.h>
#include <sys/shm.h>

#include "server.h"

#define SHARE_MEMORY_FILE_PATH \
  "/Users/chengjunjie/code/OS_study/multi_server/share_mem.lock"
#define FTOK_ID 7
void *shm_address = 0;
int global_shm_id = -1;

struct share_statistics {
  char title[20];
  int requests;
  int code_2xx;
  int code_3xx;
  int code_4xx;
  int code_5xx;
};

void do_call(int connfd);

void server(u_int16_t port) {
  // 防止产生僵尸进程
  struct sigaction action;
  action.__sigaction_u.__sa_handler = SIG_IGN;
  action.sa_flags = SA_NOCLDWAIT;
  sigaction(SIGCHLD, &action, NULL);

  // 创建XSI IPC共享存错
  key_t shm_key = ftok(SHARE_MEMORY_FILE_PATH, FTOK_ID);
  if (shm_key == -1)
    err_sys(
        "does not exist or if it cannot be accessed by the calling process");
  int shm_id = shmget(shm_key, 8192, IPC_CREAT | IPC_EXCL | 0660);
  if (shm_id == -1) err_sys("call shmget error");
  shm_address = shmat(shm_id, 0, 0);
  global_shm_id = shm_id;
  fprintf(stderr, "shm_address: %p\n", shm_address);

  // struct share_statistics
  struct share_statistics ss;
  strncpy((char *)&ss.title, "Server Statistics", 20);
  ss.requests = ss.code_2xx = ss.code_3xx = ss.code_4xx = ss.code_5xx = 0;
  memcpy(shm_address, &ss, sizeof(struct share_statistics));
  shmdt(shm_address);

  int serverfd, connfd;
  pid_t pid;
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
    if ((pid = fork()) > 0) {
      fprintf(stderr, "process pid: %d\n", getpid());
      close(connfd);
    } else if (pid == 0) {
      fprintf(stderr, "child pid: %d\n", getpid());
      close(serverfd);
      do_call(connfd);
      close(connfd);
      return;
    } else {
      err_sys("Fork error");
    }
  }
}

void do_call(int connfd) {
  struct req_line *req_buf = calloc(1, sizeof(struct req_line));
  char *recv_buffer = calloc(1, BUFFSIZE);
  char *send_buffer = calloc(1, BUFFSIZE);
  struct share_statistics *share_address = 0;

  key_t shm_key = ftok(SHARE_MEMORY_FILE_PATH, FTOK_ID);
  if (shm_key == -1) {
    fprintf(stderr,
            "call ftok error: %s does not exist or if it cannot be accessed by "
            "the calling process",
            SHARE_MEMORY_FILE_PATH);
    goto end;
  }
  int shm_id = shmget(shm_key, 0, IPC_CREAT | 0660);
  if (shm_id == -1) {
    fprintf(stderr, "do_call: call shmget error: %s\n", strerror(errno));
    goto end;
  }
  share_address = (struct share_statistics *)shmat(shm_id, 0, 0);
  if (share_address == 0) {
    fprintf(stderr, "call shmat error: %s\n", strerror(errno));
    goto end;
  }
  // print title
  fprintf(stderr, "title: %s\n", share_address->title);

  // Read Request-Line
  // Request-Line format = Method SP Request-URI SP HTTP-Version CRLF
  if (IS_ERROR ==
      read_request_line(connfd, req_buf, recv_buffer, send_buffer)) {
    goto end;
  }
  if (req_buf->method != GET) {
    response(connfd, 405, "Method Not Allowed", send_buffer, 0, 0);
    goto end;
  }
  if (req_buf->version.major == -1 || req_buf->version.major == -1) {
    response(connfd, 505, "HTTP Version Not Supported", send_buffer, 0, 0);
    goto end;
  }
  // fprintf(stderr, "%s\n", req_buf->req_uri);
  parse_uri(connfd, req_buf, send_buffer);
  // const char *mesg = "Hello world!";
  // response(connfd, 200, "Ok", send_buffer, mesg, strlen(mesg));
end:
  if (share_address != 0) shmdt(share_address);
  fprintf(stderr, "share_address: %p\n", share_address);
  free(req_buf);
  free(recv_buffer);
  free(send_buffer);
  fprintf(stderr, "fd: %d\n", connfd);
  shutdown(connfd, SHUT_RDWR);
}

int read_request_line(int connfd, struct req_line *req_line, char *recv_buffer,
                      char *send_buffer) {
  unsigned char c = 0;
  int ret = 0;
  enum req_state { method = 0, url, version, done };
  int recv_num = 0;

  enum req_state state = method;
  for (;;) {
    if ((ret = recv(connfd, &c, 1, 0)) <= 0) goto ERROR;
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
        } else if (c == SP && recv_num + 1 < URI_LIMIT) {
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
        if (c == LF) return 0;
        goto ERROR;
        break;
      default:
        goto ERROR;
        break;
    }
  }
ERROR:
  // fprintf(stderr, "state: %d\n", state);
  response(connfd, 400, "Bad request", send_buffer, 0, 0);
  return IS_ERROR;
}

void response(int connfd, int status_code, const char *reason_phrase,
              char *send_buffer, const void *message, size_t m_size) {
  int send_num = 0;
  send_num = snprintf(send_buffer, BUFFSIZE, "HTTP/1.1 %d %s%s", status_code,
                      reason_phrase, CRLF);
  send(connfd, send_buffer, send_num, 0);
  // send headers
  send_num = snprintf(send_buffer, BUFFSIZE,
                      "Content-Type: text/html; charset=utf-8%s", CRLF);
  send(connfd, send_buffer, send_num, 0);

  // // 分割换行
  // send_num = snprintf(send_buffer, BUFFSIZE, "%s", CRLF);
  // send(connfd, send_buffer, send_num, 0);

  // send body
  send_num =
      snprintf(send_buffer, BUFFSIZE, "%s%s", (const char *)message, CRLF);
  send(connfd, send_buffer, send_num, 0);
  // end
  send_num = snprintf(send_buffer, BUFFSIZE, "%s", CRLF);
  send(connfd, send_buffer, send_num, 0);
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

void parse_uri(int connfd, struct req_line *req_buffer, char *send_buffer) {
  int ret, fd;
  char *uri = req_buffer->req_uri;
  int i = 0;
  for (; i < URI_LIMIT && uri[i] != 0; ++i) {
    if (uri[i] == '?') {
      uri[i] = 0;
      break;
    }
  }
  fprintf(stderr, "i%d: %s\n", i, uri + 1);
  if (i > 0) {
    // read file
    fd = open(uri + 1, O_RDONLY, 0);
    if (fd == -1) {
      const char *mesg = strerror(errno);
      response(connfd, 404, "Not found", send_buffer, mesg, strlen(mesg));
      return;
    }
    int send_num = 0;
    send_num =
        snprintf(send_buffer, BUFFSIZE, "HTTP/1.1 %d %s%s", 200, "Ok", CRLF);
    send(connfd, send_buffer, send_num, 0);
    // send headers
    // send_num = snprintf(send_buffer, BUFFSIZE,
    //                     "%s", CRLF);
    // send(connfd, send_buffer, send_num, 0);

    while ((ret = read(fd, send_buffer, BUFFSIZE)) != 0) {
      fprintf(stderr, "ret: %d\n", ret);
      if (ret == -1 && errno == EINTR)
        continue;
      else if (ret == -1) {
        const char *mesg = strerror(errno);
        // response(connfd, 500, "Server error", send_buffer, mesg,
        // strlen(mesg));
        return;
      }
      send(connfd, send_buffer, ret, 0);
    }
    send_num = snprintf(send_buffer, BUFFSIZE, "%s", CRLF);
    send(connfd, send_buffer, send_num, 0);

    // end
    send_num = snprintf(send_buffer, BUFFSIZE, "%s", CRLF);
    send(connfd, send_buffer, send_num, 0);
    return;
  }
  response(connfd, 200, "Ok", send_buffer, 0, 0);
}

void *exit_handler(int signo) {
  shmctl(global_shm_id, IPC_RMID, NULL);
  fprintf(stderr, "end\n");
  exit(signo);
}

int main(int argc, char **argv) {
  if (argc < 2) return 1;
  uint16_t port = (uint16_t)atoi(argv[1]);
  signal(SIGINT, exit_handler);
  server(port);
  return 0;
}

