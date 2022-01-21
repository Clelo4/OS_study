/**
 * @file atomic.cpp
 * @author Jack (chengjunjie.jack@gmail.com)
 * @brief
 * @version 0.1
 * @date 2022-01-20
 *
 * @copyright Copyright (c) 2022
 */

#include <atomic>
#include <iostream>
#include <mutex>
#include <thread>

using namespace std;

void atomic_flag_test() {
  atomic_flag af(true);
  af.test_and_set();
  af.clear();
}

int main() { return 0; }
