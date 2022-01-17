/**
 * @file deque.cpp
 * @author your name (you@domain.com)
 * @brief
 * @version 0.1
 * @date 2022-01-17
 *
 * @copyright Copyright (c) 2022
 *
 */

#include <algorithm>
#include <cstdio>
#include <deque>
#include <iostream>
#include <tuple>

using namespace std;

/**
 * 特点：
 * 1. deque是双端队列
 * 2. 大小可以动态修改
 * 3. 支持直接随机访问
 * 4. 在首尾进行插入、删除非常高效
 * 5. 不保证底层数据的存储是连续的
 * 6. deque的内部实现比vector更复杂，但在某些场景下，特别是长序列，
 *    deque会更加高效些，比较重新分配内存是个昂贵的操作
 */

/**
 * 重点：
 * 1. assign()：使用新数据替换原有的数据，会修改deque的大小
 * 2. size()：返回当前deque元素数量
 * 3. max_size()：返回当前deque的最大容量
 * 4. swap()：交换两个deque的内容
 */

int main() {
  deque<int> di;
  di.push_back(1);
  di.push_back(12);
  di.push_front(2);
  di.push_back(2);

  for_each(begin(di), end(di), [](const int& item) { cout << item << endl; });
  di.clear();  // 清空所有的元素
  cout << "di.size(): " << di.size() << endl;
  for_each(begin(di), end(di), [](const int& item) { cout << item << endl; });
  di.assign(1, 101);  // 会修改deque的大小，增大or缩小
  for_each(begin(di), end(di),
           [](const int& item) -> void { cout << item << endl; });
  di.insert(di.begin(), 12, 12);
  cout << "di.size(): " << di.size() << endl;
  cout << di.max_size() << endl;
  deque<int> di2;
  di2.operator=(di);
  for_each(begin(di2), end(di2), [](const int& item) { cout << item << endl; });
  di2.resize(5);
  for_each(begin(di2), end(di2), [](const int& item) { cout << item << endl; });
  cout << "di2.size(): " << di2.size() << endl;
  di2.shrink_to_fit();
  cout << "swap" << endl;
  swap(di2, di);
  for_each(begin(di2), end(di2), [](const int& item) { cout << item << endl; });
  return 0;
}
