/**
 * @file right_reference.cpp
 * @author chengjunjie (chengjunjie.jack@gmail.com)
 * @brief
 * https://isocpp.org/blog/2012/11/universal-references-in-c11-scott-meyers
 * http://thbecker.net/articles/rvalue_references/section_01.html
 * @version 0.1
 * @date 2022-01-17
 *
 * @copyright Copyright (c) 2022
 *
 */

#include <iostream>
#include <string>
#include <type_traits>
#include <vector>

using namespace std;

// Universal reference. Because it is a type deduction of T&&.
template <typename T>
void fcn1(T &&){};

// Universal reference. Because it is a type deduction of T&&.
template <typename T>
T fcn2(T &&val) {
  return val;
};

template <typename T>
void fcn3(T) {}

int fcn4() { return 4; }

// normal rvalue reference, because is has a fully specified parameter type.
void fcn5(int &&) {}

// normal rvalue reference, because is doesn't have a type decuction of T&&.
template <typename T>
void fcn6(std::vector<T> &&params) {}

// normal rvalue reference, because is doesn't have a type decuction of T&&.
// Universal references can only occur in the form “T&&”!  Even the simple
// addition of a const qualifier is enough to disable the interpretation of “&&”
// as a universal reference:
template <typename T>
void fcn7(const T &&param) {}

template <class T, class Allocator = allocator<T> >
class vector {
 public:
  // push_back can’t exist without the class std::vector<T> that contains it.
  // But if we have a class std::vector<T>, we already know what T is, so
  // there’s no need to deduce it.
  void push_back(T &&x);  // fully specified parameter type ⇒ no type deduction;
                          // && ≡ rvalue reference
};

int main() {
  int i = 1, &li = i, &&ri = 12;
  const int &cli = i;
  // int &&rri = ri;
  const int &&cri = 13;
  typedef int &&rf_int_type;
  rf_int_type b = 12;
  // fcn1传入右值，模版参数T被推断为int类型
  fcn1(12);
  fcn1(cli * 12);
  fcn3(cli * 12);

  // fcn1传入左值引用，T& &&会折叠成类型T&
  fcn1(li);
  // fnc1传入const T&，const T& &&会折叠成类型const T&
  fcn1(cli);

  // 将左值传递给函数fcn1的右值引用形参，\且此右值引用形参指向模版类型参数时\
  编译器推断模版类型参数为实参的左值引用类型。这里模版参数T的类型为int&
  fcn1(i);
  // i被赋值后，返回左值引用
  fcn1(i = cli);
  decltype(i = cli) a = i;  // a的类型是int&

  // Bearing in mind: the lvalueness or rvalueness of an expression is
  // independent of its type.
  fcn1(ri);   // ri is an lvalue, but its type is rvalue reference
  fcn1(cri);  // cri is an lvalue, but its type is rvalue reference
  fcn1(std::move(ri));
  fcn1(std::move(string("byte")));
  fcn1(fcn4());
  fcn3(ri);
  if (is_same<decltype(fcn2(b)), int &>::value) {
    cout << "same." << endl;
  }
  // 引用折叠只能适用于间接创建的引用的引用，如类型别名或模版参数
  // 例如：rf_int_type &&会自动折叠成int&&
  if (is_same<int &&, rf_int_type &&>::value) {
    cout << "'int &&' and 'rf_int_type &&' is same." << endl;
  }

  auto &&x1ri = ri;
  auto &&x2ri = std::move(ri);
  auto x3ri = ri;
  auto x4ri = std::move(ri);

  int &&x5ri = std::move(ri);

  fcn5(std::move(5));

  auto params = std::vector<int>(10, 0);
  fcn6(std::vector<int>(10, 0));
  fcn6(std::move(params));  // “&&” means rvalue reference
  fcn7(std::move(params));  // “&&” means rvalue reference
}
