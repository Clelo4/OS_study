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

atomic<bool> ready{false};
atomic<int> data{0};

void thread1() { data.store((1, )) }
void thread2() {}

int main() { return 0; }
