/**
 * @file 07.cpp
 * @author Jack
 * @mail chengjunjie.jack@gmail.com
 * @date 2021-12-23
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
  Base() {}
  virtual ~Base() = 0;
};

Base::~Base() { log<std::string>("~Base()"); }

class C : public Base {
 public:
  C() {}
  ~C() { log<std::string>("~C()"); }
};

void fn() {
  Base* ptr = new C();
  delete ptr;
};

int main() {
  fn();
  return 0;
}
