/**
 * @file mutex.cpp
 * @author chengjunjie (chengjunjie@gmail.com)
 * @brief
 * @version 0.1
 * @date 2022-01-19
 *
 * @copyright Copyright (c) 2022
 *
 */

#include <iostream>
#include <mutex>
#include <thread>

using namespace std;

// std::mutex::lock
/**
 * 1. If the mutex isn't currently locked by any thread, the calling thread
 * locks it (from this point, and until its member unlock is called, the thread
 * owns the mutex).
 * 2. If the mutex is currently locked by another thread, execution of the
 * calling thread is blocked until unlocked by the other thread (other
 * non-locked threads continue their execution).
 * 3. If the mutex is currently locked by the same thread calling this function,
 * it produces a deadlock (with undefined behavior). See recursive_mutex for a
 * mutex type that allows multiple locks from the same thread.
 *
 */

static int global_val = 0;
recursive_mutex mtx;

void fn(int i) {
  if (i == 10) {
    cout << "lock again" << endl;
    mtx.lock();
  }
  mtx.lock();
  cout << ++global_val << endl;
  mtx.unlock();
  if (i == 10) {
    cout << "unlock again" << endl;
    mtx.unlock();
  }
}

int main() {
  for (int i = 0; i <= 10; ++i) {
    thread t1(fn, i);
    thread t2(fn, i);
    thread t3(fn, i);
    t1.join();
    t2.join();
    t3.join();
  }
  return 0;
}
