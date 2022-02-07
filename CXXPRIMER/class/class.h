#ifndef __CLASS_H
#define __CLASS_H

class A {
 public:
  A();
  A(int);
  static int static_int;
  const static int const_static_int = 103;
};

template <typename T>
class B {
 public:
  B();
  B(const A&);
  static int static_int;
};

template <typename T>
B<T>::B(const A& a) {}

template <typename T>
B<T>::B() {}

template <typename T>
int B<T>::static_int = 1;

template <typename T>
class C {
 public:
  C();
  explicit C(const B<T>&);
  static int static_int;
};

#include "template_c_implement.tpp"

// Reference:
// https://stackoverflow.com/questions/495021/why-can-templates-only-be-implemented-in-the-header-file

#endif  // __CLASS_H