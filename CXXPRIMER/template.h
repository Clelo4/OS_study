/**
 * @file template.h
 * @author Jack
 * @mail chengjunjie.jack@gmail.com
 * @date 2022-01-13
 * @version 0.1
 *
 * @copyright Copyright (c) 2022
 */

#ifndef __TEMPLATE_H
#define __TEMPLATE_H

#include <vector>

template <typename T, typename VT = std::vector<T>>
class A {
 public:
  class A_nest {};
  T t;
  VT vt;
  template <typename It>
  void test(const It& a);
  static T static_var;
};

template <typename T, typename VT>
template <typename It>
void A<T, VT>::test(const It& a) {}

#endif  // __TEMPLATE_H