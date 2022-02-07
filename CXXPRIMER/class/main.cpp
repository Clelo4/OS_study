#include <cstdio>

#include "class.h"

int main() {
  A a;
  A a1(1);
  printf("%p\n", &a.const_static_int);
  printf("%p\n", &a1.static_int);
  // B<int> b(a);
  B<int> b;
  C<int> c;
  printf("c.static_int: %p\n", &c.static_int);
  printf("C<int>::static_int: %p\n", &C<int>::static_int);
  return 0;
}