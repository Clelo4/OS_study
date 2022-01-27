/**
 * @file union.cpp
 * @author Jack (chengjunjie.jack@gmail.com)
 * @brief
 * @version 0.1
 * @date 2022-01-24
 *
 * @copyright Copyright (c) 2022
 */

#include <cstddef>
#include <cstdio>
#include <iostream>

/**
 * union的特点：
 * 1. 任何时刻只允许一个成员可用。
 * 2. union对象的空间取决于最大的成员类型。
 * 3. 不能含有引用类型
 * 4. 可以含有包括构造函数、析构函数在内的成员函数
 * 5. 不能作为基类，也不能被继承，自然不能含有虚函数
 * 6. C++11新标准：含有构造函数/析构函数的类类型可以作为成员类型
 */

union U1 {
 public:
  uint64_t a1;
  uint32_t a2;
  int64_t a3;
  uint64_t get() { return a1; }

 private:
  bool a4;
};

class A {
 public:
  union {
    int64_t a;
    uint32_t b;
  };
};

int main() {
  using namespace std;
  auto s = U1();
  s.a1 = UINT64_MAX;
  cout << s.a1 << " " << s.a3 << " " << INT64_MAX << endl;
  cout << s.get() << endl;
  cout << sizeof(U1) << endl;
  auto a_t_a = &A::a;
  auto a_t_b = &A::b;
  A a;
  a.*a_t_a = INT32_MAX + 1;
  cout << a.*a_t_a << endl;
  cout << a.*a_t_b << endl;
  return 0;
};
