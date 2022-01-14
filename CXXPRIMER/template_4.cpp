/**
 * @file template_4.cpp
 * @author Jack
 * @mail chengjunjie.jack@gmail.com
 * @date 2022-01-13
 * @version 0.1
 *
 * @copyright Copyright (c) 2022
 */
#include <algorithm>
#include <iostream>
#include <memory>
#include <type_traits>
#include <vector>

using namespace std;

// template <typename T>
// void fn(const T* a, const T* b) {
//   cout << a << endl;
//   cout << b << endl;
// }

template <typename T>
void fn(const T& a, const T& b) {
  cout << a << endl;
  cout << b << endl;
}

double d;
float f;
char c;
template <typename T>
T calc(T arg1, int arg2) {
  return arg1;
};
template <typename T>
T fcn(T arg1, T arg2) {
  return arg1;
}

template <typename T1, typename T2, typename T3>
T1 sum(T2 a, T3 b) {
  return 1;
}

template <typename It>
auto fcn1(It beg, It end) -> decltype(*beg) {
  return *beg;
}
// auto会忽略顶层const
template <typename It>
auto fcn2(It beg, It end) -> typename remove_reference<decltype(*beg)>::type {
  return *beg;
}
// auto会忽略顶层const
template <typename It>
auto fcn3(It beg, It end) -> decltype(*beg + 0) {
  return *beg;
}

template <typename T>
int fcn4(const T&, const T&) {
  return 0;
}

int main() {
  // fn("ads", "sdfdsf");
  // c风格字符串被视为char[]类型，如果模版函数形参含有引用类型，数组不会转成指针类型
  // void fn<char [5]>(const char (&a)[5], const char (&b)[5])
  fn("abcd", "abcd");
  calc(c, 'c');
  // fcn(d, f);

  //
  int i = 0, *p1 = &i;
  const int* cp1 = &i;
  // fcn(p1, cp1);
  sum<int>(1, 2);
  // max(1, 2.1);  // no matching function for call to 'max'
  vector<int> vt1;
  if (is_same<const int&, decltype(fcn1(vt1.cbegin(), vt1.cend()))>::value) {
    cout << "The return type of fcn1(vt1.cbegin(), vt1.cend()) is const int&"
         << endl;
  };
  if (is_same<int, decltype(fcn2(vt1.cbegin(), vt1.cend()))>::value) {
    cout << "The return type of fcn2(vt1.cbegin(), vt1.cend()) is int" << endl;
  };
  if (is_same<int, decltype(fcn3(vt1.cbegin(), vt1.cend()))>::value) {
    cout << "The return type of fcn3(vt1.cbegin(), vt1.cend()) is int" << endl;
  };
  int (*pf1)(const int&, const int&) = fcn4;
  return 0;
}
