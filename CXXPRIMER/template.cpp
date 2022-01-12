/le template.cpp
 * @author Jack
 * @mail chengjunjie.jack@gmail.com
 * @date 2022-01-12
 * @version 0.1
 *
 * @copyright Copyright (c) 2022
 */

#include <cstddef>
#include <iostream>

using namespace std;

template <typename T>
void template_fn_1(const T& a) {
  cout << a << endl;
}

template <size_t N>
void template_fn_1(int (
    &a)[N]) {  // a的类型必须是数组的引用类型，不能是数组类型，如果是数组类型，实参等价于一个指针，而不是数组
  for (auto i : a) cout << i << endl;
}

void template_fn_3(double (&a)[5]) { cout << sizeof(a) << endl; }

template <typename T>
class A {
 public:
  A() = default;
  ~A() = default;
  A(const T& a) : val(a) {}
  A(const A& a) : val(a.val){};
  A& operator++();
  A& operator--();
  A operator++(int);
  A operator--(int);
  void print() { cout << val << endl; }

  T val = 0;  // 类内初始化
};

template <typename T>
A<T>& A<T>::operator++() {
  ++val;
  return *this;
}
template <typename T>
A<T>& A<T>::operator--() {
  --val;
  return *this;
}
template <typename T>
A<T> A<T>::operator++(int) {
  A ret = *this;
  ++*this;
  return ret;
}
template <typename T>
A<T> A<T>::operator--(int) {
  A ret = *this;
  --*this;
  return ret;
}

int main() {
  template_fn_1<double>(12);
  double arr1[] = {1, 2, 3, 4, 5};
  template_fn_3(arr1);
  A<int> a1(121);
  ++a1;
  a1++.print();
  a1.print();
  return 0;
}**
 * @file template.cpp
 * @author Jack
 * @mail chengjunjie.jack@gmail.com
 * @date 2022-01-12
 * @version 0.1
 *
 * @copyright Copyright (c) 2022
 */

