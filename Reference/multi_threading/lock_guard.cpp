/**
 * @file lock_guard.cpp
 * @author chengjunjie (chengjunjie@gmail.com)
 * @brief
 * @version 0.1
 * @date 2022-01-20
 *
 * @copyright Copyright (c) 2022
 *
 */

#include <iostream>
#include <mutex>

/**
 * lock_guard与unique_lock类似，但有些不同：
 * 1. lock_guard中：mutex自lock_guard对象创建后就一直锁住，
 *    直到lock_guard对象销毁才解锁mutext
 * 2. unique_lock对象可以调用lock()和unlock()
 *
 */

using namespace std;

int count;
mutex mtx;

void fn() {
  lock_guard<mutex> lck(mtx);
  count++;
  cout << "cout: " << count << endl;
  return;
}

int main() {
  fn();
  fn();
  fn();
  return 0;
}
