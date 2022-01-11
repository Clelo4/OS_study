/**
 * @file main.cpp
 * @author Jack
 * @mail chengjunjie.jack@gmail.com
 * @date 2021-12-30
 * @version 0.1
 *
 * @copyright Copyright (c) 2021
 */

#include <iostream>

void Literals() {
  std::cout << "(char) a: " << sizeof('a') << std::endl;
  std::cout << "(Unicode 16 character) u'a': " << sizeof(u'a') << std::endl;
  std::cout << "(Unicode 32 character) U'a': " << sizeof(U'a') << std::endl;
  std::cout << "(wide character) L'a': " << sizeof(L'a') << std::endl;
  std::cout << "(utf-8 string literals only)u8Hello: " << sizeof(u8"Hello")
            << std::endl;
  std::cout << "(Minimum Type: unsigned) 1u: " << sizeof(1u) << std::endl;
  std::cout << "(Minimum Type: unsigned) 1U: " << sizeof(1U) << std::endl;
  std::cout << "(Minimum Type: long) 1l: " << sizeof(1l) << std::endl;
  std::cout << "(Minimum Type: long) 1L: " << sizeof(1L) << std::endl;
  std::cout << "(Minimum Type: long long) 1ll: " << sizeof(1ll) << std::endl;
  std::cout << "(Minimum Type: long long) 1LL: " << sizeof(1LL) << std::endl;
  std::cout << "(Float) 1.0f: " << sizeof(1.0f) << std::endl;
  std::cout << "(Float) 1.0F: " << sizeof(1.0F) << std::endl;
  std::cout << "(Long double) 1.0l: " << sizeof(1.0l) << std::endl;
  std::cout << "(Long double) 1.0L: " << sizeof(1.0L) << std::endl;
  std::cout << "3.14e2L: " << 3.14e2L << std::endl;
}

void Initializers() {
  class B {
   public:
    explicit B() : a(0) { std::cout << "default B()" << std::endl; }
    explicit B(int arg) : a(arg) { std::cout << "B(int arg)" << std::endl; }
    explicit B(const B& b) : a(b.a) { std::cout << "B(const B&)" << std::endl; }
    B& operator=(const B& b) {
      std::cout << "B& operator=(const B&)" << std::endl;
      return *this;
    }
    int a;
  };
  B b;  // 执行默认初始化
  /**
   * 错误的初始化方式
   * warning: empty parentheses interpreted as a function declaration
   */
  // B b1();
  B c(1);  // 执行copy初始化
  b = c;   // 执行赋值操作
  B& c1 = c;
  B d(c1);  // 执行copy初始化
  B d1{c};  // 执行copy初始化
  B d2{1};  // 执行直接初始化
  B d3{1};  // 执行直接初始化
  /**
   * 列表初始化
   * 存在narrowing conversion，报错
   */
  // B d4 = {1.1};
}

int main() {
  int a = 12;
  long b = a;  // 1212;
  // Literals();
  Initializers();
  return 0;
}
