/**
 * @file volatile.cpp
 * @author Jack (chengjunjie.jack@gmail.com)
 * @brief
 * @version 0.1
 * @date 2022-01-25
 *
 * @copyright Copyright (c) 2022
 */

// #include <cassert>
// #include <cstdio>

// class A {
//  public:
//   A() = default;
//   A(volatile const A& a){};
//   A& operator=(volatile const A& a) { return *this; };
//   volatile A& operator=(volatile const A& a) volatile { return *this; };
// };

int main() {
  // volatile A a;
  // volatile A b;
  // b = a;
  volatile int i;
  while (i)
    ;
  int b = 12;
  return 0;
}