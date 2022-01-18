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
  // at()与operator[]的区别是：at()会进行边界检查，超出边界会跑出out_of_range异常
  assert(t.at(0) == t.operator[](0));
  t.resize(1);
  cout << "size: " << t.size() << endl;
  cout << "capacity: " << t.capacity() << endl;
  t.shrink_to_fit();
  cout << "capacity: " << t.capacity() << endl;
  // 用新内容替换原有的全部内容，同时也会调整容器大小
  t.assign({1, 2, 3, 4, 5, 6, 7, 8, 9, 0});
  cout << "capacity: " << t.capacity() << endl;
  t.pop_back();
  t.push_back(12);
  cout << "capacity: " << t.capacity() << endl;
  // 原本capacity：10，增加一个元素后，容量翻倍
  t.push_back(11);
  cout << "capacity: " << t.capacity() << endl;
  t.erase(t.end()--);
  cout << "capacity: " << t.capacity() << endl;
  return 0;
}
