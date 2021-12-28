/**
 * @brief 避免遮掩继承而来的名称
 * @file 33.cpp
 * @author Jack
 * @mail chengjunjie.jack@gmail.com
 * @date 2021-12-26
 * @version 0.1
 *
 * @copyright Copyright (c) 2021
 */

#include <iostream>

class Base {
 private:
  int x;

 public:
  Base() {}
  virtual void mf1() = 0;
  virtual void mf1(int) {}
  virtual void mf2() {}
  void mf3() {}
  void mf3(double) {}
  void mf4() {}
  void mf4(double) {}
};

class Derived : public Base {
 public:
  Derived() : Base() {}
  virtual void mf1() {
    // 调用父类的mf1(int)
    Base::mf1(12);
  }
  virtual void mf2() {}
  // 遮盖住了从父类继承来的的mf3()和mf3(double)
  void mf3() {}
  using Base::mf4;
  void mf4() {}
};

int main() {
  Derived d;
  d.mf1();
  return 0;
}
