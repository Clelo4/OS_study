/**
 * @file test.cpp
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
#include <typeinfo>

using namespace std;

class A {
 public:
  virtual ~A(){};
};
class B : public A {
 public:
  virtual ~B() {}
};
class C : public B {};
class D : public B, public A {};

int main() {
  A* a = new C();  // A类型必须含有虚函数
  auto da = dynamic_cast<B*>(a);
  B* b = new B();
  auto dc = dynamic_cast<C*>(b);
  assert(dc == nullptr);
  try {
    auto ddc = dynamic_cast<C&&>(B());
  } catch (bad_cast e) {
    cout << e.what() << endl;
  }
  A* pa = new D();

  return 0;
}
