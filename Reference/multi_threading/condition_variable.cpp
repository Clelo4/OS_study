/**
 * @file condition_variable.cpp
 * @author Jack (chengjunjie.jack@gmail.com)
 * @brief
 * @version 0.1
 * @date 2022-01-20
 *
 * @copyright Copyright (c) 2022
 */

#include <condition_variable>
#include <iostream>
#include <mutex>
#include <thread>

using namespace std;

mutex mtx;
condition_variable cv;
bool ready = false;

void print_id(int id) {
  unique_lock<mutex> lck(mtx);
  while (!ready) cv.wait(lck);
  cout << "thread " << id << endl;
}

void go() {
  unique_lock<mutex> lck(mtx);
  ready = true;
  cv.notify_all();
}

int main() {
  thread threads[10];
  for (int i = 0; i < 10; ++i) {
    threads[i] = thread(print_id, i);
  }
  cout << "10 threads ready to race...\n";
  go();
  for (auto& th : threads) th.join();
  return 0;
}
