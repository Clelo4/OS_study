/**
 * @file default_constructor.cpp
 * @author Jack (chengjunjie.jack@gmail.com)
 * @brief
 * @version 0.1
 * @date 2022-01-24
 *
 * @copyright Copyright (c) 2022
 */

#include <iostream>

using namespace std;

class A {
 public:
  int &i;
  A() =
      default;  // 引用成员i没有类内初始化，即使我们使用default，编译器任然将其implicitly定义为删除的

 private:
};

int main() {
  //
  // 使用()调用默认构造函数是错误用法
  // 因为这是declaring a function a() that returns a base object.
  // A a();
  // decltype(a) bb;
  // a();
  // cout << a.i << endl;
  // A a1;  // error: use of deleted function ‘A::A()’
  int b = 12;
  A a2{i : b};
  return 0;
};
