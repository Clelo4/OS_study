/**
 * @file Exception.cpp
 * @author Jack (chengjunjie.jack@gmail.com)
 * @brief
 * @version 0.1
 * @date 2022-01-26
 *
 * @copyright Copyright (c) 2022
 */

#include <cstdio>
#include <exception>
#include <iostream>

using namespace std;

// 异常对象（一种特殊的对象）
// 1. 编译器使用异常抛出表达式来对异常对象进行拷贝初始化
//    所以异常对象必须包含可访问的拷贝构造/移动构造函数，且必须含有一个可访问的析构函数
// 2. 如果异常对象是数组类型或函数类型，则转为相对应的指针类型，
//    编译器对指针进行拷贝初始化
// 3. 异常对象存储在编译器管理的空间内，任何catch语句均能访问它，
//    异常处理完后，该异常对象被销毁
// 4. 在抛出指针时，需要注意

class A {
 public:
  A() = default;
  A(const A& a) = delete;
  A(const A&& a){};
  explicit A(int k_) try : key(k_) {
  } catch (...) {
    cerr << "catch explicit A(int k_)" << endl;
  }
  ~A() {
    cerr << "~A(" << key << ")" << endl;
    key = -1;
  }

  int key = 0;
};
class B {
 public:
  B() = default;
  explicit B(int k_) try : key(k_) {
  } catch (...) {
    cerr << "catch explicit B(int k_)" << endl;
  }
  ~B() {
    cerr << "~B(" << key << ")" << endl;
    key = -1;
  }

  int key = 0;
};

enum EXC_TYPE { INT, CLASS_A, POINTER, NULL_PTR, ARRAY };

void exception_object(EXC_TYPE e_type) {
  A a;
  int b[12] = {};
  switch (e_type) {
    case INT:
      throw 12;
      break;
    case CLASS_A:
      throw a;
      break;
    case NULL_PTR:
      throw nullptr;  // e
      break;
    case POINTER:
      // 异常对象是函数，转为相对应的指针
      throw exception_object;
      break;
    case ARRAY:
      // 异常对象是数组，转为相对应的指针
      throw b;  // b是局部对象，而且无法复制数组。显然，访问一个指向局部作用域的指针是会导致严重错误
      break;
    default:
      break;
  }
};

// 1. noexcept指明函数不会抛出异常
// 2. 如果noexcept函数内部抛出异常，程序会调用terminate函数中止整个程序，
//    以确保遵守不在运行时抛出异常的承诺.
// 3. 声明函数不会抛出异常的价值：
//    a. 函数始终不会抛出异常
//    b. 函数可能抛出异常，但我们不知道如何处理这种异常
// 4. 向后兼容：下面两种情况是等价的
//    a. C++11之前：void fn() throw() {}
//    b. C++11：void fn() noexcept {}
// 5. noexcept参数
//    a. noexcept(true) 声明函数不会抛出异常
//    b. noexcept(false) 声明函数可能会抛出异常
// 6. noexcept运算符...

void noexcept_fn() noexcept { throw 1; }

using right_ref_A = A&&;

int main() {
  range_error r("range_error");
  try {
    // exception_object(ARRAY);
    exception* p = &r;
    throw *p;
  } catch (exception e) {
    printf("exception: %s\n", e.what());
  } catch (exception* e) {
    // e如果是指向range_error的指针
    printf("exception*: %s\n", e->what());
  } catch (nullptr_t e) {
    printf("nullptr_t: %p\n", e);
  } catch (decltype(exception_object) e) {
    printf("function: %p\n", e);
  } catch (int* e) {
    printf("int*: %p\n", e);
  }
  try {
    {
      A a(1);
      {
        A a(2);
        cerr << "&a: " << &a << endl;
        throw a;
        throw &a;  // 错误用法，不能抛出指向局部对象的指针
      }
    }
    // noexcept_fn();
  } catch (A& e) {
    cerr << "catch(A& e): " << &e << endl;
  } catch (A* e) {
    cerr << "catch(A* e): " << e->key << endl;
  } catch (...) {
    cerr << "noexcept_fn catch(...)" << endl;
  }
  try {
    try {
      B b(101);
      throw b;
    } catch (B e) {
      cerr << "inside catch(B& e): " << e.key << " " << &e << endl;
      throw;  // 进行重抛出，抛出的异常对象依旧是编译器存储空间的异常对象，而不是当前局部对象e
    }
  } catch (B& e) {
    cerr << "outside catch(B& e): " << e.key << " " << &e << endl;
  }
  // catch子句中的异常声明，形参不能是右值引用
  // 因为：在throw抛出表达式中，编译器已经对异常对象进行copy初始化，
  // 也就是说catch子句中的异常对象实参，其实是存储在编译器管理的内存空间中，
  // 显然无法对这样的对象进行移动操作
  return 0;
}
