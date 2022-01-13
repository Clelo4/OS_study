/**
 * @file template_2.cpp
 * @author Jack
 * @mail chengjunjie.jack@gmail.com
 * @date 2022-01-13
 * @version 0.1
 *
 * @copyright Copyright (c) 2022
 */
#include <iostream>
#include <string>
#include <vector>

using namespace std;

template <typename T, typename VT = vector<T>>
class A {
 public:
  class A_nest {};
  T t;
  VT vt;
};

void fn() {
  A<int> a;
  // 必须使用typename，而不是class，以此来显式声明A_next是个类类型
  typename A<int>::A_nest a_n;
  A<int, string> a1;
  cout << a1.vt << endl;
}

int main() {
  fn();
  return 0;
}
