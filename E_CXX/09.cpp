/**
 * @brief 绝不在构造和析构过程中调用virtual函数
 * @file 09.cpp
 * @author Jack
 * @mail chengjunjie.jack@gmail.com
 * @date 2021-12-25
 * @version 0.1
 *
 * @copyright Copyright (c) 2021
 */

#include <iostream>
#include <string>

template <typename T>
void log(const T& t) {
  std::cout << t << std::endl;
}

class Base {
 public:
  Base() {
    // 不用构造函数中调用虚函数
    this->virtualFn();
  }
  virtual void virtualFn();
  virtual ~Base() = 0;
};

void Base::virtualFn() { log<std::string>("Base virtualFn"); }
Base::~Base() {
  log<std::string>("~Base()");
  // 不用解构函数中调用虚函数
  this->virtualFn();
}

class C : public Base {
 public:
  C() {}
  virtual void virtualFn() { log<std::string>("C virtualFn"); }
  ~C() { log<std::string>("~C()"); }
};

void fn() {
  Base* ptr = new C();
  ptr->virtualFn();
  delete ptr;
};

int main() {
  fn();
  return 0;
}
