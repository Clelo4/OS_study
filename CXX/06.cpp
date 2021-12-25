/**
 * @file 06.cpp
 * @author Jack
 * @mail chengjunjie.jack@gmail.com
 * @date 2021-12-23
 * @version 0.1
 *
 * @copyright Copyright (c) 2021
 */

#include <iostream>

class Base {
 public:
  Base() {}
  ~Base() {}

 private:
  // 禁止compiler自动生成下列函数（只需要声明函数即可）
  Base(const Base&);
  Base& operator=(const Base&);
};

class Uncopyable {
 protected:
  Uncopyable() {}
  ~Uncopyable() {}

 private:
  Uncopyable(const Uncopyable&);
  Uncopyable& operator=(const Uncopyable&);
};

class C : private Uncopyable {
 public:
  C() {}
};

int main() {
  Base b1;
  C c1;
}
