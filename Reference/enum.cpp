/**
 * @file enum.cpp
 * @author chengjunjie (chengjunjie@gmail.com)
 * @brief
 * @version 0.1
 * @date 2022-01-21
 *
 * @copyright Copyright (c) 2022
 *
 */

#include <atomic>
#include <cstdio>
#include <iostream>

using namespace std;

class A {
 public:
  enum E1 : int;  // 前置声明（非限定作用域必须制定成员类型）
  enum E1 : int {  // 定义
    E1_1,
    E1_2 = 1,
    E1_3 = 1,
  };
  enum class E2 : uint64_t;  // 前置声明
  enum class E3;   // 前置声明（限定作用域默认类型为int）
  enum class E3 {  // 枚举定义（限定作用域默认类型为int）
    E3_1,
  };
  enum class E4 {  // 限定作用域枚举默认类型为int
    // E4_1 = UINT64_MAX; // 枚举值不在其基础类型("int")的范围内
  };
  enum {
    NONE_ENUM_1,
    NONE_ENUM_2,
    NONE_ENUM_3 = UINT64_MAX,
  } unnamed_enum_1;
};
// 限定作用域枚举类外定义
enum class A::E2 : uint64_t {
  E2_1,
  E2_2 = 1,
  E2_3 = UINT64_MAX,
};

int main() {
  // A::E1_1;
  A::E1 a_e1 = A::E1_1;
  A::E2 a_e2 = A::E2::E2_3;
  A a;
  a.unnamed_enum_1 = A::NONE_ENUM_1;
  cout << 1ul << endl;
  printf("%llu\n", a_e2);
  printf("%lu\n", sizeof(a.unnamed_enum_1));
  // cout << a_e1 << " " << a_e2 << endl;
  return 0;
}