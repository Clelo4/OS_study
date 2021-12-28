/**
 * @brief 复制对象时，需要复制其每一部分，包括其父类对象
 * @file 12.cpp
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
  Base() { log<std::string>("construct Base"); }
  Base(const Base& b) { log<std::string>("copy construct Base"); }
  Base& operator=(const Base&) {
    log<std::string>("copy assignment Base");
    return *this;
  }
  virtual ~Base() = 0;
};

Base::~Base() {
  // log<std::string>("~Base()");
}

class C : public Base {
 public:
  C() { log<std::string>("construct C"); }
  C(const C& c)
      : Base(c)  // 手动调用base class的copy构造函数，否则会调用构造函数
  {
    log<std::string>("copy construct C");
  }
  C& operator=(const C& c) {
    Base::operator=(c);  // 手动调用base class的copy assignment函数
    log<std::string>("copy assignment C");
    return *this;
  }
  ~C() {
    // log<std::string>("~C()");
  }
};

void fn() {
  C c1;
  C c2(c1);   // copy初始化
  C c3 = c2;  // copy初始化
  c3 = c1;
};

int main() {
  fn();
  return 0;
}
