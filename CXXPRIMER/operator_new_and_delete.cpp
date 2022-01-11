/**
 * @file operator_new_and_delete.cpp
 * @author Jack
 * @mail chengjunjie.jack@gmail.com
 * @date 2022-01-11
 * @version 0.1
 *
 * @copyright Copyright (c) 2022
 */

#include <unistd.h>

#include <cassert>
#include <cstdlib>
#include <exception>
#include <iostream>
#include <memory>
#include <string>

class Base {
 public:
  int a = 12;  // 类内初始值

  Base() = default;
  ~Base() { printf("Call Base::~Base()\n"); }
  explicit Base(int arg1) : a(arg1){};
  // 隐式static，隐式inline
  void *operator new(size_t sz) {
    printf("Base::operator new(%zu)\n", sz);
#ifdef __cplusplus
    return ::operator new(sz);
#else
    if (void *mem = malloc(sz)) {
      return mem;
    } else
      throw std::bad_alloc();
#endif
  }
  // 隐式static，隐式inline
  void operator delete(void *mem) noexcept {
    if (mem == nullptr) return;
    printf("Base::operator delete(%p)\n", mem);
#ifdef __cplusplus
    ::operator delete(mem);
#else
    free(mem);
#endif
  }
  // 隐式static，隐式inline
  void *operator new(size_t sz, void *mem) {
    printf("Base::operator new(%zu, %p)\n", sz, mem);
    return ::operator new(sz, mem);
  }
};

int main() {
  void *void_base_ptr = nullptr;
  Base *base_ptr = nullptr;
#ifdef USE_CUSTOM_OPERATOR_NEW_DELETE
  // 调用Base::operator new(size_t)，创建原始内存
  void_base_ptr = Base::operator new(sizeof(Base));
  // 该placement new表达式运作原理：1. 调用operator new(size_t, void*)
  // 获得能容纳Base的内存地址;
  // 2. 利用上一步获取的内存地址，允许Base的构造函数；3. 返回构造后的指针
  base_ptr = new (void_base_ptr) Base(101);
#else
  base_ptr = new Base(101);
#endif

  assert(base_ptr->a == 101);

#ifdef USE_CUSTOM_OPERATOR_NEW_DELETE
  base_ptr->~Base();  // 手动调用析构函数
  // 调用Base::operator delete，释放原始内存
  Base::operator delete(void_base_ptr);
#else
  delete base_ptr;
#endif
  printf("end\n");
  return 0;
}
