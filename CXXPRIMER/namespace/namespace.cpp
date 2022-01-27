/**
 * @file namespace.cpp
 * @author Jack (chengjunjie.jack@gmail.com)
 * @brief
 * @version 0.1
 * @date 2022-01-27
 *
 * @copyright Copyright (c) 2022
 */

#include <algorithm>
#include <iostream>
#include <map>
#include <string>
#include <vector>

#include "un1.h"
#include "un2.h"

// extern static int e_s_i; // 错误用法
static int static_int_1 =
    1;  // static or thread storage duration and internal linkage.
extern int
    extern_int_;  // static or thread storage duration and external linkage.

static thread_local int thread_local_static_int_1 = 12;

namespace wrapper {
// 内联命名空间
inline namespace inline_namespace_1 {
int i_n_1 = 102;
}
}  // namespace wrapper

namespace my_name {
namespace nested_name {
static int a = 12;
int b = 13;
}  // namespace nested_name
namespace std1 {}
using namespace std;
// #include <cassert> // 不能在命名空间内使用#include
}  // namespace my_name

namespace std {
// 特例化类模版（部分特例化）
template <typename T>
class vector<map<string, T>> {};
// 特例化类模版（全特化）
template <>
class vector<int> {};
}  // namespace std

int main() {
  using namespace std;
  using namespace my_name::nested_name;
  cout << a << " " << b << endl;
  vector<int> a;
  vector<double> b;
  vector<map<string, int>> c;
  b.push_back(12);
  // 显式使用全局命名空间
  void *ptr = ::operator new(1024);
  ::operator delete(ptr);
  // 显式使用inline空间的对象
  cout << wrapper::inline_namespace_1::i_n_1 << endl;
  // 使用inline空间的对象
  cout << wrapper::i_n_1 << endl;
  cout << thread_local_static_int_1 << endl;
  print1();
  print2();
  return 0;
}
