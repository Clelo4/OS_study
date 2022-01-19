/**
 * @file stack.cpp
 * @author your name (you@domain.com)
 * @brief
 * @version 0.1
 * @date 2022-01-19
 *
 * @copyright Copyright (c) 2022
 *
 */

#include <iostream>
#include <stack>
#include <vector>

using namespace std;

int main() {
  stack<int, vector<int>> si;
  si.push(12);
  auto si2(si);
  si2.push(1231);
  si.top();
  swap(si, si2);
  return 0;
}
