#include <iostream>
#include <type_traits>

using namespace std;

template <typename T>
void fcn1(T &&){};
template <typename T>
T fcn2(T &&val) {
  return val;
};
template <typename T>
void fcn3(T) {}

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
  // fnc1传入const T&，模版参数T被推断为const T&类型
  fcn1(cli);

  // 将左值传递给函数fcn1的右值引用形参，\且此右值引用形参指向模版类型参数时\
  编译器推断模版类型参数为实参的左值引用类型。这里模版参数T的类型为int&
  fcn1(i);
  // i被赋值后，返回左值引用
  fcn1(i = cli);
  decltype(i = cli) a = i;  // a的类型是int&

  fcn1(std::move(ri));
  fcn3(ri);
  if (is_same<decltype(fcn2(b)), int &>::value) {
    cout << "same" << endl;
  }
  // 引用折叠只能适用于间接创建的引用的引用，如类型别名或模版参数
  // 例如：rf_int_type &&会自动折叠成int&&
  if (is_same<int &&, rf_int_type &&>::value) {
    cout << "int &&, rf_int_type &&" << endl;
  }
}
