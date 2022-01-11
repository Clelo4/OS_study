/**
 * @file 1.cpp
 * @author Jack
 * @mail chengjunjie.jack@gmail.com
 * @date 2021-12-28
 * @version 0.1
 *
 * @copyright Copyright (c) 2021
 */
#include <algorithm>
#include <cstdio>
#include <iostream>
#include <string>
#include <type_traits>
#include <vector>

class B;

class A {
 public:
  A(int a, int b = 1, int c = 1) { printf("a: %d, b: %d\n", a, b); }
  friend class B;

 protected:
  int prot_mem = 1;

 private:
  int private_mem = 2;
};

class B : private A {
 public:
  B(int a) : A(a) {}
  using A::private_mem;
  using A::prot_mem;
};

void fn(const A &arg){};
void fn() {
  static int count = -1;
  ++count;
  std::cout << "fn count: " << count << std::endl;
};

// 64位系统
#include <stdio.h>
struct {
  int x;
  char y;
} s;

int main() {
  int a = 1;
  int *b = &a;
  if (std::is_same<decltype(*b), int &>::value) {
    std::printf("decltype(*b) is same as int&\n");
  }
  if (std::is_same<decltype((a)), int &>::value) {
    std::printf("decltype((a)) is same as int&\n");
  }
  std::string s = "A";
  for (auto &c : s) {
    if (std::is_same<decltype(c), char &>::value) {
      std::printf("decltype(c) is same as char&\n");
    }
  }
  std::vector<int> is;

  if (std::is_same<decltype(is.begin() - is.end()),
                   std::vector<int>::difference_type>::value) {
    std::printf(
        "decltype(is.begin() - is.end()) is same as "
        "std::vector<int>::difference_type\n");
  }
  int arr1[] = {1, 2, 3, 4};
  decltype(std::begin(arr1)) b_ptr;
  std::vector<int> vi(std::begin(arr1), std::end(arr1));
  fn(1);
  A a1 = {1, 2};
  B b1{1};
  std::cout << "b1.prot_mem: " << b1.prot_mem << std::endl;
  std::cout << "b1.private_mem: " << b1.private_mem << std::endl;
  for (int i = 0; i < 10; ++i) fn();
  printf("%d\n", sizeof(s));  // 输出24
  return 0;
}
