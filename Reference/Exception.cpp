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

// throw抛出异常对象。
// 编译器使用异常抛出表达式来对异常对象进行拷贝初始化
// 所以异常对象必须是可拷贝/可移动，且必须含有一个可访问的析构函数

// 如果异常对象是数组类型或函数类型，则转为相对应的指针类型

class A {
 public:
  A() = default;
  A(const A& a) = delete;
  A(const A&& a){};
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

int main() {
  range_error r("range_error");
  try {
    // exception_object(ARRAY);
    exception* p = &r;
    // 当我们抛出*p，即使p指向子类对象，
    // 由于会进行复制/移动操作，则会按照*p的静态类型进行复制/移动。
    // 也就是说：只有*p的当前静态类型部分被抛出。
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
  return 0;
}