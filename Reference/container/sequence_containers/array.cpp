/**
 * @file array.cpp
 * @author chengjunjie (chengjunjie.jack@gmail.com)
 * @brief
 * @version 0.1
 * @date 2022-01-17
 *
 * @copyright Copyright (c) 2022
 *
 */

/**
 * 数组特点：
 *  1. 固定大小，不可修改
 *  2. 支持随机访问
 *  3. 可以通过data()获取底层指针
 *  4. size()等于max_size()
 */

#include <array>
#include <cstdio>
#include <iostream>
#include <tuple>

using namespace std;

const int arr_size = 10;
typedef array<int, arr_size> array_type_1;

// Member types
array_type_1::reference;
array_type_1::value_type;
array_type_1::const_reference;
array_type_1::pointer;
array_type_1::const_pointer;
array_type_1::iterator;
array_type_1::const_iterator;
array_type_1::reverse_iterator;
array_type_1::const_reverse_iterator;
array_type_1::size_type;
array_type_1::difference_type;

int main() {
  array_type_1 a1 = {12, 123, 12};
  a1.size();
  a1.max_size();
  a1[1];
  array_type_1::value_type* first_pointer = a1.data();
  printf("first_pointer: %p\n", first_pointer);
  a1.fill(1024);
  array_type_1 a2;
  a2.fill(512);
  a1.swap(a2);
  for (int i = 0; i != a1.size(); ++i) {
    printf("%d\n", *first_pointer);
    ++first_pointer;
  }
  // 获取第一个元素
  get<0>(a1);
  // 获取最后一个元素
  get<a1.size() - 1>(a1);
  return 0;
};