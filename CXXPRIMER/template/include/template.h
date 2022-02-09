/**
 * @file template.h
 * @author Jack
 * @mail chengjunjie.jack@gmail.com
 * @date 2022-02-08
 * @version 0.1
 *
 * @copyright Copyright (c) 2022
 */

#ifndef TEMPLATE_H_
#define TEMPLATE_H_

#include <cstdio>
#include <vector>

template <typename T, typename VT = std::vector<T>>
class Base {
 public:
  class A_nest {};
  T t;
  VT vt;
  template <typename It>
  void test(const It& a);
  virtual void g() { printf("Base\n"); }
  static T static_var;
};

template <typename T, typename VT = std::vector<T>>
class Base_derived : public Base<T, VT> {
 private:
  using Base<T, VT>::g;

 public:
  T t;
  VT vt;
  void fn();
  void g() { printf("Base_derived\n"); }
};

template <typename T, typename VT>
template <typename It>
void Base<T, VT>::test(const It& a) {}

template <typename T, typename VT>
void Base_derived<T, VT>::fn() {
  // 1. 添加Base<T, VT>::的原因：
  //  a. The compiler does not look in dependent base classes when looking up
  //     nondependent names.
  //  b. Base是dependent names，A_nest是nondependent names.
  // 2. 添加typename的原因：
  //  因为存在potential specialization，
  //  编译器在知道T、VT的真实类型前，不会假设A_nest是类型
  typename Base<T, VT>::A_nest a;
  printf("Base_derived<T, VT>::fn() -> sizof(a): %lu\n", sizeof(a));

  // Here’s the rule: the compiler does not look in dependent base classes (like
  // Base_derived<T, VT>) when looking up nondependent names (like g).
  // g();
  this->g();
  g();
  Base<T, VT>::g();  // 不推荐使用Base<T, VT>的形式，virtual机制会失效
}

template <typename T, typename VT = std::vector<T>>
class Base_derived_two : public Base_derived<T, VT> {
 public:
  void g() { printf("Base_derived_two\n"); }
};

#endif  // TEMPLATE_H_