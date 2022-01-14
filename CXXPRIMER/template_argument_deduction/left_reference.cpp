#include <iostream>

template <typename T>
void fcn1(T&){};
template <typename T>
void fcn2(const T&){};

int main() {
  int i = 1, &li = i, &&ri = 12;
  // fcn1只能传入左值
  fcn1(i);
  fcn1(li);
  // fnc1能传入左值、右值
  fcn2(12);
  fcn2(i);
  fcn2(ri);
}
