/**
 * @file unique_lock.cpp
 * @author Jack
 * @mail chengjunjie.jack@gmail.com
 * @date 2022-01-20
 * @version 0.1
 *
 * @copyright Copyright (c) 2022
 */

#include <iostream>
#include <mutex>
#include <thread>

using namespace std;
mutex mtx;

/**
 * 1. unique_lock能在automatic duration内，维护mutex的locked和unlocked状态
 * 2. 类似unique_ptr，当automaticduration结束时，mutex自动解锁：
 *     a. 局部作用域结束时
 *     b. 发生异常时
 * 3. unique_lock不维护mutex的声明周期，
 *    应确保mutex至少在unique_lock对象销毁还一直存在
 *
 */

int count;

template <typename T>
void print_cout_and_unlock(T* p__mtx) {
  cout << "cout: " << count << endl;
  p__mtx->unlock();
}

void fn() {
  unique_lock<mutex> lck(mtx);
  count++;
  print_cout_and_unlock(lck.release());
  return;
}

int main() {
  fn();
  fn();
  fn();
  return 0;
}
