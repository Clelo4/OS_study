/**
 * @brief 若某个函数的任意参数进行类型转换，那么这个函数必须是个non-memeber
 * @file 21.cpp
 * @author Jack
 * @mail chengjunjie.jack@gmail.com
 * @date 2021-12-26
 * @version 0.1
 *
 * @copyright Copyright (c) 2021
 */

#include <iostream>
#include <string>

template <typename T>
void log(const T& t) {
  std::cout << t << std::endl;
}

class Rational {
 public:
  Rational(int numerator = 0, int denominator = 1)
      : n(numerator), d(denominator) {}
  Rational(const Rational& r) : n(r.n), d(r.d) {
    log<std::string>("copy constructor");
  }
  Rational& operator=(const Rational& b) {
    log<std::string>("copy assignment");
    n = b.n;
    d = b.d;
    return *this;
  }
  int numerator() const { return n; }
  int denominator() const { return d; }
  void print() {
    log<int>(n);
    log<int>(d);
  }
  // 不可执行2 * a操作
  // const Rational operator*(const Rational& rhs) const {
  //   return Rational(this->n * rhs.numerator(), this->d * rhs.denominator());
  // }

 private:
  int n, d;
};

// 可执行2 * a操作
const Rational operator*(const Rational& lhs, const Rational& rhs) {
  log<std::string>("Non-member operator* start");
  Rational a(lhs.numerator() * rhs.numerator(),
             lhs.denominator() * rhs.denominator());
  log<std::string>("Non-member operator* end");
  return a;
}

int main() {
  Rational a(1, 2);
  Rational b(3, 5);
  Rational c = 2 * a;
  c.print();
  return 0;
}
