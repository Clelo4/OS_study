/**
 * @file template_2.cpp
 * @author Jack
 * @mail chengjunjie.jack@gmail.com
 * @date 2022-01-13
 * @version 0.1
 *
 * @copyright Copyright (c) 2022
 */
#include <cstdio>
#include <iostream>
#include <string>
#include <vector>

#include "template.h"

using namespace std;

extern template class A<int>;
template class A<vector<int>>;

class NoDefault {
  // NoDefault() = delete;
};

namespace std {
template class vector<NoDefault>;
}
template class A<NoDefault>;

void fn() {
  A<int> a;
  // 必须使用typename，而不是class，以此来显式声明A_next是个类类型
  typename A<int>::A_nest a_n;
  A<int, string> a1;
  a1.vt = "ABC";
  cout << a1.vt << endl;
  cout << sizeof(class A<vector<int>>) << endl;
  cout << sizeof(class A<vector<double>>) << endl;
}

template <typename T>
class Stack {};
void f1(Stack<char>);
class Extercise {
  Stack<double> &rsd;
  Stack<int> si;
};

int main() {
  fn();
  int iObj = sizeof(Stack<string>);
  printf("A_static_int: %p\n", &A<int>::A_static_int);
  return 0;
}
