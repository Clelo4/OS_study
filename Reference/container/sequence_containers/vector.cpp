/**
 * @file vector.cpp
 * @author your name (you@domain.com)
 * @brief
 * @version 0.1
 * @date 2022-01-18
 *
 * @copyright Copyright (c) 2022
 *
 */

#include <cassert>
#include <iostream>
#include <vector>

/**
 * vector特点：
 * 1. 支持随机访问
 * 2. 支持动态插入、删除元素，但支持在尾部进行操作
 *
 */

using namespace std;

using vector_t = vector<int>;

int main() {
  vector_t t = {1, 2, 3, 4, 4, 5};
  assert(t.at(0) == t.operator[](0));
  return 0;
}
