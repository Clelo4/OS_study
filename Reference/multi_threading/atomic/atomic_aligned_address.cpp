/**
 * @file atomic_aligned_address.cpp
 * @author Jack (chengjunjie.jack@gmail.com)
 * @brief
 * @version 0.1
 * @date 2022-01-25
 *
 * @copyright Copyright (c) 2022
 */

#include <atomic>
#include <cstddef>
#include <iostream>

using namespace std;

// __uint128_t：the standard does not define a 128-bit integer type. It's an
// extension that some compilers provide and others don't.

//

int main() {
  // 分配内存
  char *area = nullptr;
  area = static_cast<char *>(calloc(1024, sizeof(char)));
  char *test_mem = area + 61;
  // 将内存地址转为数字
  uintptr_t test_mem_num = reinterpret_cast<uintptr_t>(test_mem);
  printf("%p %lu\n", test_mem, test_mem_num % 8);
  // 这里的atomic_ptr真的是lock-free？
  atomic<__uint128_t> *atomic_ptr = new (test_mem) atomic<__uint128_t>(1);
  printf("%d\n", atomic_ptr->is_lock_free());
  return 0;
};
// 参考：https://groups.google.com/a/isocpp.org/g/std-discussion/c/DfFCC2UWAvI
