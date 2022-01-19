/**
 * @file operator_convert.cpp
 * @author Jack
 * @mail chengjunjie.jack@gmail.com
 * @date 2022-01-16
 * @version 0.1
 *
 * @copyright Copyright (c) 2022
 */

#include <cstddef>
#include <iostream>

class SmallInt {
  friend SmallInt operator+(const SmallInt&, const SmallInt&);

 public:
  SmallInt(int i = 0) : val(i) {}
  operator int() const { return val; };

 private:
  std::size_t val;
};

SmallInt operator+(const SmallInt& a, const SmallInt& b) {
  std::cout << "SmallInt operator+(const SmallInt& a, const SmallInt& b)"
            << std::endl;
  return SmallInt(a.val + b.val);
}

struct LongDouble {
  LongDouble(double i = 0.0) : val(i) {}
  operator double() { return static_cast<double>(val); }
  operator float() { return static_cast<float>(val); }
  // operator int() { return static_cast<int>(val); }
  LongDouble operator+(const SmallInt&) {
    std::cout << "LongDouble operator+(const SmallInt&)" << std::endl;
    return *this;
  };
  long double val;
};
LongDouble operator+(LongDouble&, double) {
  std::cout << "LongDouble operator+(LongDouble&, double)" << std::endl;
  return LongDouble();
}

void calc(int){};
void calc(LongDouble){};

int main() {
  SmallInt s1, s2;
  SmallInt s3 = s1 + s2;
  // int i = s3 + 0;

  LongDouble ld;
  // 有歧义：LongDouble可以转换double或float，但从double、float转到int都是算术类型转换。
  // int ex1 = ld;
  float ex2 = ld;

  double dval;
  calc(dval);  // 匹配到void calc(int)，因为算术类型转换优先于类类型转换

  LongDouble ld1;
  SmallInt sm1;
  // error: use of overloaded operator '+' is ambiguous (with operand types\
  'SmallInt' and 'LongDouble' ld1 = sm1 + ld1;
  ld1 = ld1 + sm1;

  double d = operator+(s1, 3.14);
  // s1 + 3.14;

  return 0;
}
