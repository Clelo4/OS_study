/**
 * @brief 一个swap函数
 * @file 21.cpp
 * @author Jack
 * @mail chengjunjie.jack@gmail.com
 * @date 2021-12-26
 * @version 0.1
 *
 * @copyright Copyright (c) 2021
 */

#include <iostream>
#include <list>
#include <string>
#include <vector>

template <typename T>
void log(const T& t) {
  std::cout << t << std::endl;
}

namespace WidgetStuff {

class WidgetImpl {
 public:
 private:
  int a, b, c;
  std::vector<double> v;
};

template <typename T>
class Widget {
 public:
  Widget() {}
  Widget(const Widget<T>& rhs);
  Widget& operator=(const Widget<T>& rhs) {
    *pImpl = *(rhs.pImpl);
    return *this;
  }
  void swap(Widget<T>& other) {
    using std::swap;
    swap(pImpl, other.pImpl);
  }

 private:
  WidgetImpl* pImpl;
};

// template <typename T>
// Widget<T>::Widget() {}

template <typename T>
void swap(Widget<T>& a, Widget<T>& b) {
  a.swap(b);
}

}  // namespace WidgetStuff

namespace std {
template <>  // 这是对std::swap的total template specialization
void swap<WidgetStuff::WidgetImpl>(WidgetStuff::WidgetImpl& a,
                                   WidgetStuff::WidgetImpl& b) noexcept {
  // a.swap(b);
}
}  // namespace std

namespace std {
// error: function template partial specialization is not allowed
// template <typename T>
// void swap<std::vector<T> >(std::vector<T>& a, std ::vector<T>& b) {}

// class template partial specialization is allowed
template <typename T>
class std::vector<std::list<T> > {};
}  // namespace std

namespace std {
// std::swap的一个重载版本：不合法！！！
// template <typename T>
// void swap(T& a, T& b) {}
}  // namespace std

template <typename T>
void doSomething(T& obj1, T& obj2) {
  using std::swap;  // 令std::swap在此函数内可用
  // 根据ADL（argument-dependent lookup）规则，会选择合适的swap版本
  swap(obj1, obj2);
}

int main() {
  WidgetStuff::Widget<int> a, b;
  swap(a, b);
  doSomething(a, b);
}
// See also
// https://en.cppreference.com/w/cpp/language/adl