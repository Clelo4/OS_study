/**
 * @file multi_server_pass_socket_fd_to_child_processes.c
 * @author Jack
 * @mail chengjunjie.jack@gmail.com
 * @date 2021-12-16
 * @version 0.1
 *
 * @copyright Copyright (c) 2021
 */

#include <assert.h>
#include <dirent.h>
#include <fcntl.h>
#include <semaphore.h>
#include <signal.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/stat.h>
#include <sys/un.h>

#include "server.h"

#define SHARE_MEMORY_FILE_PATH "./share_mem.lock"
#ifdef __linux__
#define SEM_FILE "/sem_lock"
#define SEM_USER1_FILE "/sem_user1_lock"
#else
#define SEM_FILE SHARE_MEMORY_FILE_PATH
#define SEM_USER1_FILE "./sem_user1.lock"
#endif
#define FTOK_ID 8
#define SHM_SIZE 8192
#define UNIX_SOCKET_NAME "/var/tmp/unix_socket_name.socket"

int global_shm_id = -1;
struct share_statistics *share_address = 0;
sem_t *shm_sem = 0;
sem_t *user1_sem = 0;
int alarm_count = 0;
int need_stop_script = 0;
struct share_statistics {
  char title[20];
  int requests;
  int code_2xx;
  int code_3xx;
  int code_4xx;
  int code_5xx;
};

void handle_bash(int connfd, char *send_buffer, char *command);
/**
 * Create a server endpoint of a connection.
 * Return fd is all OK, < 0 on error.
 */
int un_listen(const char *name);

void write_code(int code) {
  sem_wait(shm_sem);
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
  sem_post(shm_sem);
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

int pipe_read_v2(const char *command, int *script_pid) {
  int pipe_fd[2];
  int pid = -1;
  if (-1 == pipe(pipe_fd)) {
    log_print("calll pipe error %s\n", strerror(errno));
    return -1;
  }
  if ((pid = fork()) < 0)
    return -1;
  else if (pid == 0) {
    setpgid(0, 0);
    close(pipe_fd[0]);
    dup2(pipe_fd[1], STDOUT_FILENO);
    dup2(pipe_fd[1], STDERR_FILENO);
    log_print("command: %s\n", command);
    execl("/bin/bash", "bash", "-c", command, (char *)0);
    _exit(127);
  } else {
    *script_pid = pid;
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

void do_call(int connfd) {
  struct req_line *req_buf = calloc(1, sizeof(struct req_line));
  char *recv_buffer = calloc(1, BUFFSIZE);
  char *send_buffer = calloc(1, BUFFSIZE);

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
    sem_wait(shm_sem);
    int total = share_address->requests;
    sem_post(shm_sem);
    char mesg_buff[200];
    snprintf(mesg_buff, 200, "requests: %d\n", total);

    response(connfd, 200, "Ok", send_buffer, mesg_buff, strlen(mesg_buff));
  } else if (req_buf->req_uri[0] != 0 && req_buf->req_uri[5] == '/' &&
             strncasecmp("bash", req_buf->req_uri + 1, 4) == 0) {
    handle_bash(connfd, send_buffer, req_buf->req_uri + 6);
  } else if (is_dir(req_buf->req_uri + 1)) {
    // ls_version_one(connfd, send_buffer, req_buf->req_uri + 1);
    ls_version_two(connfd, send_buffer, req_buf->req_uri + 1);
  } else
    req_parser(connfd, req_buf, send_buffer);
end:
  free(req_buf);
  free(recv_buffer);
  free(send_buffer);
  shutdown(connfd, SHUT_WR);
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
  log_print("url: %s\n", file_path);
  if (i > 0) {
    if (faccessat(AT_FDCWD, file_path, F_OK, AT_EACCESS) == -1) {
      // ???????????????
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
      log_print("ret: %d\n", ret);
      if (ret == -1 && errno == EINTR)
        continue;
      else if (ret == -1) {
        const char *mesg = strerror(errno);
        log_print("%s\n", mesg);
        return;
      }
      send(connfd, send_buffer, ret, 0);
    }

    // end
    // send_num = snprintf(send_buffer, BUFFSIZE, "%s", CRLF);
    // send(connfd, send_buffer, send_num, 0);
    return;
  }
  response(connfd, 200, "Ok", send_buffer, 0, 0);
}

void server(u_int16_t port) {
  log_print("start server: %d\n", getpid());
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

  if (-1 == setsockopt(serverfd, SOL_SOCKET, SO_REUSEPORT, &on,
                       (socklen_t)sizeof(on)))
    err_sys("setsockopt SO_REUSEPORT error");

  if (-1 == bind(serverfd, (const struct sockaddr *)&serveraddr,
                 (socklen_t)sizeof(serveraddr)))
    err_sys("bind socket fd error");
  if (-1 == listen(serverfd, 0)) err_sys("listen socket error");

  shm_sem = sem_open(SHARE_MEMORY_FILE_PATH, O_CREAT, S_IRWXU, 1);
  if (shm_sem == SEM_FAILED) {
    log_print("SEM_FAILED: %d sem_open: %p %s\n", SEM_FAILED, shm_sem,
              strerror(errno));
    exit(1);
  }
  share_address = (struct share_statistics *)shmat(global_shm_id, 0, 0);
  if (share_address == 0) {
    log_print("call shmat error: %s\n", strerror(errno));
    exit(1);
  }
  log_print("do_call: %d title: %s\n", getpid(), share_address->title);

  while (1) {
    if ((connfd = accept(serverfd, (struct sockaddr *)&clientaddr,
                         (socklen_t *)&len)) < 0) {
      if (errno == EINTR) continue;
      err_sys("socket accept error");
    }
    do_call(connfd);
  }
}

void siguser1_signo_handler(int signo) { sem_post(user1_sem); }

void handler_siguser1() {
  shm_sem = sem_open(SHARE_MEMORY_FILE_PATH, O_CREAT, S_IRWXU, 1);
  if (shm_sem == SEM_FAILED) {
    log_print("SEM_FAILED: %d sem_open: %p %s\n", SEM_FAILED, shm_sem,
              strerror(errno));
  }
  user1_sem = sem_open(SEM_USER1_FILE, O_CREAT, S_IRWXU, 0);
  if (user1_sem == SEM_FAILED) {
    log_print("SEM_FAILED: %d sem_open: %p %s\n", SEM_FAILED, shm_sem,
              strerror(errno));
  }
  share_address = (struct share_statistics *)shmat(global_shm_id, 0, 0);
  if (share_address == 0) {
    log_print("handler_siguser1 call shmat error: %s\n", strerror(errno));
  }
  log_print("handler_siguser1 title: %s\n", share_address->title);
  int value;
  for (;;) {
    sem_wait(user1_sem);
    sem_wait(shm_sem);
    value = share_address->requests;
    sem_post(shm_sem);
    fprintf(stderr, "handler_siguser1 requests: %d\n", value);
  }
}

void exit_handler(int signo) {
  if (global_shm_id != -1) shmctl(global_shm_id, IPC_RMID, NULL);
  const char *end = "main process end\n";
  write(STDERR_FILENO, end, strlen(end));
  exit(signo);
}

int main(int argc, char **argv) {
  uint16_t port = 2346;

  void *shm_address = 0;
  key_t shm_key;

  // ??????XSI IPC????????????
  shm_key = ftok(SHARE_MEMORY_FILE_PATH, FTOK_ID);
  if (shm_key == -1)
    err_sys(
        "SHARE_MEMORY_FILE_PATH does not exist or if it cannot be accessed "
        "by "
        "the calling process\n");
  int shm_id = shmget(shm_key, SHM_SIZE, IPC_CREAT | 0660);
  if (shm_id == -1) err_sys("call shmget error");
  shm_address = shmat(shm_id, 0, 0);
  global_shm_id = shm_id;
  log_print("shm_address: %p\n", shm_address);

  // struct share_statistics
  struct share_statistics ss;
  strncpy((char *)&ss.title, "Server Statistics", 20);
  ss.requests = ss.code_2xx = ss.code_3xx = ss.code_4xx = ss.code_5xx = 0;
  memcpy(shm_address, &ss, sizeof(struct share_statistics));
  shmdt(shm_address);

  sem_unlink(SEM_FILE);

  fprintf(stderr, "main pid: %d\n", getpid());

  // create unix socket
  struct sockaddr_un un;
  if (strlen(UNIX_SOCKET_NAME) >= sizeof(un.sun_path)) {
    errno = ENAMETOOLONG;
  }

  static pid_t process_pool[32];
  for (int i = 0; i < sizeof(process_pool) / sizeof(pid_t); ++i) {
    pid_t pid = fork();
    if (pid == 0) {
      server(port);
      exit(1);
    } else if (pid < 0) {
      err_sys("create process error\n");
    }
    process_pool[i] = pid;
  }

  sem_unlink(SEM_USER1_FILE);

  user1_sem = sem_open(SEM_USER1_FILE, O_CREAT, S_IRWXU, 0);
  if (user1_sem == SEM_FAILED) {
    log_print("SEM_FAILED: %d user1_sem sem_open: %p %s\n", SEM_FAILED,
              user1_sem, strerror(errno));
  }
  signal(SIGINT, exit_handler);
  signal(SIGUSR1, siguser1_signo_handler);
  int handler_siguser1_pid = fork();
  if (handler_siguser1_pid == 0) {
    handler_siguser1();
    exit(127);
  } else if (handler_siguser1_pid < 0) {
    exit(1);
  }

  for (;;) {
    waitpid(-1, NULL, 0);
  }

  return 0;
}

void handle_sigalrm(int signo) { alarm_count++; }
void handle_sigalrm_stop_scipt(int signo) { need_stop_script = 1; }

void handle_bash_stop_child_process_group(int script_pid) {
  signal(SIGALRM, handle_sigalrm);
  kill(-script_pid, SIGTERM);
  alarm(5);
  while (-1 != waitpid(-script_pid, NULL, 0) && errno != ECHILD) {
    if (alarm_count != 0) kill(-script_pid, SIGKILL);
  }
  log_print("kill scirpt process: %d\n", script_pid);
}
void stop_scirpt_timeout(int script_pid) {
  alarm_count = 0;
  signal(SIGALRM, handle_sigalrm);
  kill(-script_pid, SIGTERM);
  alarm(5);
  while (-1 != waitpid(-script_pid, NULL, 0) && errno != ECHILD) {
    if (alarm_count != 0) kill(-script_pid, SIGKILL);
  }
  log_print("kill scirpt process: %d\n", script_pid);
}

void handle_bash(int connfd, char *send_buffer, char *command) {
  int script_pid = 0;
  int fd = pipe_read_v2(command, &script_pid);
  if (fd == -1) {
    response(connfd, 500, "Ok", send_buffer, "command running error",
             sizeof("command running error"));
  } else {
    int send_num = 0;
    int ret = 0;
    send_num =
        snprintf(send_buffer, BUFFSIZE, "HTTP/1.1 %d %s%s", 200, "Ok", CRLF);
    if (-1 == send(connfd, send_buffer, send_num, 0) && errno == EPIPE) {
      handle_bash_stop_child_process_group(script_pid);
      return;
    } else
      write_code(200);

    send_num = snprintf(send_buffer, BUFFSIZE, "%s", CRLF);
    if (-1 == send(connfd, send_buffer, send_num, 0) && errno == EPIPE) {
      handle_bash_stop_child_process_group(script_pid);
      return;
    }

    log_print("handle_sigalrm_stop_scipt\n");
    signal(SIGALRM, handle_sigalrm_stop_scipt);
    alarm(10);
    while ((ret = read(fd, send_buffer, BUFFSIZE)) != 0 &&
           need_stop_script == 0) {
      log_print("ret: %d\n", ret);
      if (ret == -1 && errno == EINTR)
        continue;
      else if (ret == -1) {
        const char *mesg = strerror(errno);
        log_print("%s\n", mesg);
        break;
      }
      if (-1 == send(connfd, send_buffer, ret, 0) && errno == EPIPE) {
        handle_bash_stop_child_process_group(script_pid);
        return;
      }
    }
    if (ret == -1 || need_stop_script == 1) {
      log_print("run stop_scirpt_timeout\n");
      stop_scirpt_timeout(script_pid);
    }
  }
}