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

#include <iostream>
#include <thread>

using namespace std;

void fn() {
  cout << "fn" << endl;
  return;
}

int main() {
  thread first(fn);
  thread second(fn);
  first.join();
  second.join();
  return 0;
}
