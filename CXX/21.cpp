/**
 * @file 21.cpp
 * @author Jack
 * @mail chengjunjie.jack@gmail.com
 * @date 2021-12-25
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
  explicit Rational() { log<std::string>("empty constructor"); }
  Rational(int numerator = 0, int denominator = 1)
      : n(numerator), d(denominator) {
    log<std::string>("constructor");
  }
  Rational(const Rational& r) : n(r.n), d(r.d) {
    log<std::string>("copy constructor");
  }
  Rational& operator=(const Rational& b) {
    log<std::string>("copy assignment");
    n = b.n;
    d = b.d;
    return *this;
  }
  void print() {
    log<int>(n);
    log<int>(d);
  }

 private:
  int n, d;
  friend const Rational operator*(const Rational& lhs, const Rational& rhs);
};

inline const Rational operator*(const Rational& lhs, const Rational& rhs) {
  log<std::string>("operator* start");
  Rational a(lhs.n * rhs.n, lhs.d * rhs.d);
  log<std::string>("operator* end");
  return a;
}

int main() {
  Rational a(1, 2);
  Rational b(3, 5);
  Rational c = a * b;  // 为什么不调用copy constructor？
  c.print();
  return 0;
}
