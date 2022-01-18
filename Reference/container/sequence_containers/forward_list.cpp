/**
 * @file forward_list.cpp
 * @author your name (you@domain.com)
 * @brief
 * @version 0.1
 * @date 2022-01-18
 *
 * @copyright Copyright (c) 2022
 *
 */

#include <cassert>
#include <forward_list>
#include <iostream>

using namespace std;

/**
 * forward_list特点：
 * 1. 不支持随机访问，只能从头遍历
 * 2. 与array、vector、deque相比，forward_list在插入、提取、删除元素时
 *    更加高效
 * 3. 会消耗更多的内存，如next指针
 * 4. 没有size()函数：forward_list是唯一不存在size()的standard container，
 *    原因是维护一个size变量需要消耗一些时间
 * 5. 与list相比，不支持push_back()和pop_back();
 * 6. forward_list::operator不支持算术操作，但支持自增，不支持自减
 */

int main() {
  std::forward_list<int> l1;
  // Assigns new contents to the forward_list container, \
     replacing its current contents, and modifying its size accordingly.
  l1.assign(12, 1);
  l1.insert_after(l1.before_begin(), 0);
  l1.push_front(-1);
  l1.remove(0);
  l1.push_front(2);
  l1.remove_if([](decltype(l1)::value_type& i) -> bool { return i % 2 != 0; });
  l1.reverse();
  l1.resize(100);
  l1.sort(greater<int>());
  cout << "first: " << *l1.begin() << endl;
  decltype(l1) l2;
  auto old_it = l2.begin();
  swap(l2, l1);
  assert(l1.begin() == l1.end());
  assert(old_it == l1.begin());
  return 0;
}
