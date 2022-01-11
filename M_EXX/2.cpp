/**
 * @file 1.cpp
 * @author Jack
 * @mail chengjunjie.jack@gmail.com
 * @date 2021-12-28
 * @version 0.1
 *
 * @copyright Copyright (c) 2021
 */
#include <iostream>
// dynamic_cast_3.cpp
// compile with: /c /GR
class B {
  virtual void f(){};
};
class D : public B {
 public:
  virtual void f(){};
  int a;
};

void f() {
  B* pb = new D;  // unclear but ok
  B* pb2 = new B;
  D d1;
  B b1;

  D* pd = dynamic_cast<D*>(pb);   // ok: pb actually points to a D
  D& pd2 = dynamic_cast<D&>(b1);  // pb2 points to a B not a D
  std::cout << pd << std::endl;
  // std::cout << pd2 << std::endl;
  // std::cout << &pd2->a;
}
int main() {
  f();
  return 0;
}
