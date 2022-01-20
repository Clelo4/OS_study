/**
 * @file thread.cpp
 * @author your name (you@domain.com)
 * @brief
 * @version 0.1
 * @date 2022-01-19
 *
 * @copyright Copyright (c) 2022
 *
 */

#include <cstdio>
#include <exception>
#include <iostream>
#include <thread>

using namespace std;

void fn() {
  cout << "fn" << endl;
  return;
}

// 已RAII管理线程，确保异常发生时，能够等待线程结束
class thread_guard {
 public:
  explicit thread_guard(thread& t_) : t(t_) {}
  thread_guard(const thread_guard&) = delete;
  thread_guard& operator=(const thread_guard&) = delete;
  ~thread_guard() {
    cout << "~thread_guard()" << endl;
    if (t.joinable()) t.join();
  }

 private:
  thread& t;
};

struct func {
  int& i;
  func(int& i_) : i(i_) { printf("func: %p\n", &i_); }
  void operator()(int& data_) {
    printf("operator(): %p\n", &data_);
    for (volatile unsigned j = 0; j < 1000000; ++j) {
      // do_something(i);  // 1. 潜在访问隐患：悬空引用
    }
  }
};

void fun2(int& data_) { printf("operator(): %p\n", &data_); }

void thread_guard_join() {
  int some_local_state = 12;
  printf("some_local_state: %p\n", &some_local_state);
  func my_func(some_local_state);
  int d1 = 10;
  printf("thread_data: %p\n", &d1);
  thread th(fun2, ref(d1));
  thread_guard g(th);
  throw bad_exception();
}

int main() {
  thread first(fn);
  thread second(fn);
  second.detach();
  if (first.joinable()) first.join();
  if (second.joinable()) second.join();
  try {
    thread_guard_join();
  } catch (bad_exception e) {
    cout << e.what() << endl;
  } catch (exception e) {
    cout << e.what() << endl;
  }
  return 0;
}
