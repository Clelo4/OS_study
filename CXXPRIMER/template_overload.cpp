/**
 * @file template_overload.cpp
 * @author Jack
 * @mail chengjunjie.jack@gmail.com
 * @date 2022-01-15
 * @version 0.1
 *
 * @copyright Copyright (c) 2022
 */

#include <iostream>

using namespace std;

// （函数模版与非模版函数）匹配规则：
// 1. 匹配度高优先匹配（注：这个过程会经历类型转换，而函数模版只允许两种转换）
// 2. 如果匹配度一样，则按照下面的规则进行匹配
//    a. 优先选择非模版函数
//    b. 如果没有非模版函数，选择更加特例化的函数模版
//    c. 否则此调用有歧义，无法通过编译

template <typename T>
void f(T) {
  cout << 1 << endl;
}
template <typename T>
void f(const T*) {
  cout << 2 << endl;
}
template <typename T>
void g(T) {
  cout << 3 << endl;
}
template <typename T>
void g(T*) {
  cout << 4 << endl;
}
int main() {
  int i = 42, *p = &i;
  const int ci = 9, *p2 = &ci;
  g(42);
  g(p);
  // 函数模版重载：顶层const无论在形参还是实参中都会被删除
  g(ci);
  g(p2);
  f(42);
  // 函数模版只允许两种类型转换：\
      1. 底层const转换：可以将非指向const的指针（或引用）实参，传递给指向const的指针（或引用）形参。\
      2. 函数/数组转换成指针：如果模版函数形参不是引用类型，则可以将实参转成相应的指针。
  // p是int*，有两种可行匹配：T、const T*，
  // 但由于第二种需要进行底层const转换，而第一种是精准匹配，所以T的匹配优先级更高
  f(p);
  f(ci);
  f(p2);
  return 0;
}