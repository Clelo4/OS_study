/**
 * @file explicit.cpp
 * @author Jack
 * @mail chengjunjie.jack@gmail.com
 * @date 2022-01-16
 * @version 0.1
 *
 * @copyright Copyright (c) 2022
 */

#include <iostream>

using namespace std;

class A {
 public:
  A(int v) : val(v) {}
  explicit operator int() const { return val; }
  int val;
};

int main() {
  A a0(0);
  A a1(-121);
  if (static_cast<int>(a1) + 1) {
  }
  return 0;
}
