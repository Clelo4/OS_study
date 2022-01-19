/**
 * @file priority_queue.cpp
 * @author your name (you@domain.com)
 * @brief
 * @version 0.1
 * @date 2022-01-18
 *
 * @copyright Copyright (c) 2022
 *
 */

#include <cassert>
#include <functional>
#include <iostream>
#include <queue>
#include <string>

using namespace std;

int main() {
  priority_queue<int, vector<int>, greater<int>()> pq;
  pq.push(12);
  pq.push(1);
  pq.push(2);
  pq.push(3);
  pq.push(-1);
  assert(pq.top() == -1);
  return 0;
}
