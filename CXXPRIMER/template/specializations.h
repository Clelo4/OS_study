/**
 * @file specializations.cpp
 * @author Jack (chengjunjie.jack@gmail.com)
 * @brief
 * @version 0.1
 * @date 2022-02-07
 *
 * @copyright Copyright (c) 2022
 */

#ifndef TEMPLATE_SPECIALIZATIONS_
#define TEMPLATE_SPECIALIZATIONS_

#include <iomanip>
#include <iostream>
#include <limits>
#include <sstream>
#include <string>
#include <typeinfo>

// function template
template <typename T>
void fn(const T& t) {}

// function template specialization
template <>
void fn<int>(const int& t) {}

template <>
void fn<std::string>(const std::string& t) {}

template <typename T>
inline std::string stringify(const T& x) {
  std::ostringstream out;
  out << x;
  return out.str();
}

template <>
inline std::string stringify<bool>(const bool& x) {
  std::ostringstream out;
  out << std::boolalpha << x;
  return out.str();
}

template <>
inline std::string stringify<double>(const double& x) {
  std::ostringstream out;
  const int sigdigits = std::numeric_limits<double>::digits10;
  out << std::setprecision(sigdigits) << x;
  return out.str();
}

template <>
inline std::string stringify<float>(const float& x) {
  std::ostringstream out;
  const int sigdigits = std::numeric_limits<float>::digits10;
  out << std::setprecision(sigdigits) << x;
  return out.str();
}

template <typename T>
void foo(T* x) {
  std::cout << "foo<" << typeid(T).name() << ">(T*)\n";
}

#endif  // TEMPLATE_SPECIALIZATIONS_
