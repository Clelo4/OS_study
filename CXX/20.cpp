/**
 * @file 20.cpp
 * @author Jack
 * @mail chengjunjie.jack@gmail.com
 * @date 2021-12-25
 * @version 0.1
 *
 * @copyright Copyright (c) 2021
 */

#include <iostream>
#include <string>
#include <type_traits>

typedef void (*typedef_fn_type)(void);
using using_fn_type = void (*)(void);

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
  virtual ~Base();
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

// pass-by-value
void fn(Base b) {}
// pass-by-reference-to-const
void fn1(const C& c) {}

// 推荐使用内建类型使用pass-by-value
void buildIn(int a) {}
// 推荐使用对STL的迭代器使用pass-by-value
void stdIterator(std::string::iterator it) {}
// 推荐使用函数对象使用pass-by-value
void funtionPointer(typedef_fn_type fn) {}

int main() {
  C c1;
  fn(c1);
  fn1(c1);
  return 0;
}
