/**
 * @file list.cpp
 * @author your name (you@domain.com)
 * @brief
 * @version 0.1
 * @date 2022-01-18
 *
 * @copyright Copyright (c) 2022
 *
 */

#include <algorithm>
#include <cassert>
#include <iostream>
#include <list>

/**
 * list：双向链表
 * 特点：
 * 1. 与forward_list相比，能尾部进行插入、删除操作
 * 2. list::operator能支持自增、自减
 * 3. 类似forward_list，不支持随机访问
 * 4. splice()：用于从别的对象转移内容（数据只进行转移，不复制）
 * 5. merge()：根据相应的排序位置合并别的对象到当前对象
 *    （both containers shall already be ordered）、（数据只进行转移，不复制）
 */

using list_t = std::list<int>;

int main() {
  list_t a;
  // 在指定位置插入数据，返回新插入节点
  auto it = a.insert(a.begin(), 12);
  assert(it == a.begin());
  a.push_front(11);
  a.push_front(10);
  list_t b{3, 2, 1};
  a.splice(a.begin(), b, b.begin(), b.end());
  a.sort();
  assert(*a.begin() == 1);
  assert(b.size() == 0);
  b.assign({123, 123, 123, 324, 3});
  a.sort();
  b.sort();
  // Merges x into the list by transferring all of its elements at their
  // respective ordered positions into the container (both containers shall
  // already be ordered).
  a.merge(b);
  std::for_each(a.begin(), a.end(), [](decltype(a)::value_type &i) -> void {
    std::cout << i << " ";
  });
  std::cout << std::endl;
  assert(b.size() == 0);
  return 0;
}
