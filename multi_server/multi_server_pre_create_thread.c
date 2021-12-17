/**
 * @file multi_server_thread
 * @author Jack
 * @mail chengjunjie.jack@gmail.com
 * @date 2021-12-16
 * @version 0.1
 *
 * @copyright Copyright (c) 2021
 */

#include <dirent.h>
#include <pthread.h>
#include <semaphore.h>
#include <signal.h>
#include <sys/ipc.h>
#include <sys/shm.h>

#include "server.h"

#define SHARE_MEMORY_FILE_PATH "./share_mem.lock"
#define FTOK_ID 102
#define SHM_SIZE 8192
#define N_THREADS 16
static pthread_t thread_pool[N_THREADS];
int global_shm_id = -1;
struct share_statistics *share_address = 0;
sem_t sem;

struct share_statistics {
  char title[20];
  int requests;
  int code_2xx;
  int code_3xx;
  int code_4xx;
  int code_5xx;
};

void write_code(int code) {
  sem_wait(&sem);
  ++share_address->requests;
  if (share_address) {
    switch (code / 100) {
      case 2:
        ++share_address->code_2xx;
        break;
      case 3:
        ++share_address->code_3xx;
        break;
      case 4:
        ++share_address->code_4xx;
        break;
      case 5:
        ++share_address->code_5xx;
        break;
      default:
        break;
    }
  }
}

int is_dir(const char *path) {
  struct stat ss;
  if (0 == stat(path, &ss) && S_ISDIR(ss.st_mode)) {
    return 1;
  }
  return 0;
}

int pipe_read(const char *file_path) {
  int pipe_fd[2];
  int pid = -1;
  if (-1 == pipe(pipe_fd)) {
    log_print("calll pipe error %s\n", strerror(errno));
    return -1;
  }
  if ((pid = fork()) < 0)
    return -1;
  else if (pid == 0) {
    close(pipe_fd[0]);
    dup2(pipe_fd[1], STDOUT_FILENO);
    dup2(pipe_fd[1], STDERR_FILENO);
    execl("/bin/ls", "ls", "-al", file_path, (char *)0);
    _exit(127);
  } else {
    close(pipe_fd[1]);
    return pipe_fd[0];
  }
  return -1;
}

void ls_version_one(int connfd, char *send_buffer, const char *path) {
  int fd = pipe_read(path);
  if (fd == -1) {
    response(connfd, 500, "Ok", send_buffer, "pipe read error",
             sizeof("pipe read error"));
  } else {
    int send_num = 0;
    int ret = 0;
    send_num =
        snprintf(send_buffer, BUFFSIZE, "HTTP/1.1 %d %s%s", 200, "Ok", CRLF);
    send(connfd, send_buffer, send_num, 0);
    write_code(200);

    send_num = snprintf(send_buffer, BUFFSIZE, "%s", CRLF);
    send(connfd, send_buffer, send_num, 0);

    while ((ret = read(fd, send_buffer, BUFFSIZE)) != 0) {
      log_print("ret: %d\n", ret);
      if (ret == -1 && errno == EINTR)
        continue;
      else if (ret == -1) {
        const char *mesg = strerror(errno);
        log_print("%s\n", mesg);
        break;
      }
      send(connfd, send_buffer, ret, 0);
    }

    // end
    close(fd);
  }
}

void ls_version_two(int connfd, char *send_buffer, const char *path) {
  DIR *dir = opendir(path);
  if (dir != 0) {
    struct dirent *dir_entry = 0;
    errno = 0;
    int send_num = 0;
    int ret = 0;
    send_num =
        snprintf(send_buffer, BUFFSIZE, "HTTP/1.1 %d %s%s", 200, "Ok", CRLF);
    send(connfd, send_buffer, send_num, 0);
    write_code(200);

    send_num = snprintf(send_buffer, BUFFSIZE,
                        "content-type: text/json; charset=UTF-8%s", CRLF);
    send(connfd, send_buffer, send_num, 0);

    send_num = snprintf(send_buffer, BUFFSIZE, "%s", CRLF);
    send(connfd, send_buffer, send_num, 0);
    while ((dir_entry = readdir(dir)) != 0) {
      send_num = snprintf(send_buffer, BUFFSIZE, "%s\n", dir_entry->d_name);
      send(connfd, send_buffer, send_num, 0);
    }
    send_num = snprintf(send_buffer, BUFFSIZE, "%s", CRLF);
    send(connfd, send_buffer, send_num, 0);
    if (errno) log_print("readdir %s error: \n", path, strerror(errno));
    if (closedir(dir) < 0) {
      log_print("close dir %s error: %s\n", path, strerror(errno));
    }

    // end
  } else {
    response(connfd, 500, "server error", send_buffer, "call opendir error",
             sizeof("call opendir error"));
  }
}

void *do_call(void *arg) {
  int serverfd = (long)arg;
  struct sockaddr_in clientaddr;
  socklen_t len = sizeof(clientaddr);
  int connfd = 0;
  struct req_line *req_buf = calloc(1, sizeof(struct req_line));
  char *recv_buffer = calloc(1, BUFFSIZE);
  char *send_buffer = calloc(1, BUFFSIZE);
  while (1) {
    if ((connfd = accept(serverfd, (struct sockaddr *)&clientaddr,
                         (socklen_t *)&len)) < 0) {
      if (errno == EINTR) continue;
      err_sys("socket accept error");
    }

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
    if (req_buf->req_uri[0] != 0 &&
        strncasecmp("statistics", req_buf->req_uri + 1, sizeof("statistics")) ==
            0) {
      //
      sem_wait(&sem);
      int total = share_address->requests;
      sem_post(&sem);
      char mesg_buff[200];
      snprintf(mesg_buff, 200, "requests: %d\n", total);
      response(connfd, 200, "Ok", send_buffer, mesg_buff, strlen(mesg_buff));
    } else if (is_dir(req_buf->req_uri + 1)) {
      // ls_version_one(connfd, send_buffer, req_buf->req_uri + 1);
      ls_version_two(connfd, send_buffer, req_buf->req_uri + 1);
    } else
      req_parser(connfd, req_buf, send_buffer);
  end:
    log_print("end, connfd: %d\n", connfd);
    shutdown(connfd, SHUT_RDWR);
    close(connfd);
  }
  free(req_buf);
  free(recv_buffer);
  free(send_buffer);
  pthread_exit((void *)0);
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
        if (c == LF) {
          return 0;
        }
        goto ERROR;
        break;
      default:
        goto ERROR;
        break;
    }
  }
ERROR:
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

  send_num = snprintf(send_buffer, BUFFSIZE, "%s", CRLF);
  send(connfd, send_buffer, send_num, 0);

  // send body
  send_num =
      snprintf(send_buffer, BUFFSIZE, "%s%s", (const char *)message, CRLF);
  send(connfd, send_buffer, send_num, 0);
  // end
  // send_num = snprintf(send_buffer, BUFFSIZE, "%s", CRLF);
  // send(connfd, send_buffer, send_num, 0);
  write_code(status_code);
}

void parse_method(enum http_method *method, const char *buff, size_t n) {
  unsigned char first_char = buff[0];
  *method = OTHER;
  switch (first_char) {
    case 'G':
      if (strncasecmp("GET", (const char *)buff, 3) == 0) {
        *method = GET;
      }
      break;
    case 'P':
      if (strncasecmp("POST", (const char *)buff, 3) == 0) {
        *method = POST;
      }
      break;
    default:
      break;
  }
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

void req_parser(int connfd, struct req_line *req_buffer, char *send_buffer) {
  int ret, fd;
  char *uri = req_buffer->req_uri;
  int i = 0;
  for (; i < URI_LIMIT && uri[i] != 0; ++i) {
    if (uri[i] == '?') {
      uri[i] = 0;
      break;
    }
  }
  const char *file_path = uri + 1;
  if (i > 0) {
    if (faccessat(AT_FDCWD, file_path, F_OK, AT_EACCESS) == -1) {
      // 文件不存在
      const char *mesg = strerror(errno);
      response(connfd, 404, "not found", send_buffer, mesg, strlen(mesg));
      return;
    }
    // read file
    fd = open(file_path, O_RDONLY, 0);
    if (fd == -1) {
      const char *mesg = strerror(errno);
      if (errno == EACCES)
        response(connfd, 403, "require permissions", send_buffer, mesg,
                 strlen(mesg));
      else
        response(connfd, 500, "server error", send_buffer, mesg, strlen(mesg));
      return;
    }
    int send_num = 0;
    send_num =
        snprintf(send_buffer, BUFFSIZE, "HTTP/1.1 %d %s%s", 200, "Ok", CRLF);
    send(connfd, send_buffer, send_num, 0);
    write_code(200);

    send_num = snprintf(send_buffer, BUFFSIZE, "%s", CRLF);
    send(connfd, send_buffer, send_num, 0);

    while ((ret = read(fd, send_buffer, BUFFSIZE)) != 0) {
      log_print("req_parser ret: %d\n", ret);
      if (ret == -1 && errno == EINTR)
        continue;
      else if (ret == -1) {
        const char *mesg = strerror(errno);
        log_print("%s\n", mesg);
        return;
      }
      send(connfd, send_buffer, ret, 0);
    }

    return;
  }
  response(connfd, 200, "Ok", send_buffer, 0, 0);
}

void server(u_int16_t port) {
  pthread_t thread_init;
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
  if (-1 == listen(serverfd, 20)) err_sys("listen socket error");

  for (int i = 0; i < N_THREADS; ++i) {
    pthread_create(&thread_pool[i], NULL, do_call, (void *)serverfd);
  }

  for (int i = 0; i < N_THREADS; ++i) {
    pthread_join(thread_pool[i], NULL);
  }
}

void exit_handler(void) {
  const char *end = "main process end\n";
  if (global_shm_id != -1) shmctl(global_shm_id, IPC_RMID, NULL);
  sem_destroy(&sem);
  write(STDERR_FILENO, end, strlen(end));
}

void sigint_handler(int signo) { exit(1); }

int main(int argc, char **argv) {
  signal(SIGINT, sigint_handler);
  atexit(exit_handler);

  key_t shm_key;

  // 创建XSI IPC共享存错
  shm_key = ftok(SHARE_MEMORY_FILE_PATH, FTOK_ID);
  if (shm_key == -1)
    err_sys(
        "SHARE_MEMORY_FILE_PATH does not exist or if it cannot be accessed "
        "by "
        "the calling process\n");
  int shm_id = shmget(shm_key, SHM_SIZE, IPC_CREAT | IPC_EXCL | 0660);
  if (shm_id == -1) err_sys("call shmget error");
  share_address = shmat(shm_id, 0, 0);
  global_shm_id = shm_id;

  // struct share_statistics
  struct share_statistics ss;
  strncpy((char *)&ss.title, "Server Statistics", 20);
  ss.requests = ss.code_2xx = ss.code_3xx = ss.code_4xx = ss.code_5xx = 0;
  memcpy(share_address, &ss, sizeof(struct share_statistics));

  sem_init(&sem, 0, 1);

  server(1991);
  return 0;
}
