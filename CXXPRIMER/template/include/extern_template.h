#ifndef EXTERN_TEMPLATE_H_
#define EXTERN_TEMPLATE_H_

#include <cstdio>

template <typename T>
extern void extern_fn();

// pre-declare
template <typename T>
class A;
// pre-declare
template <typename T>
void fn_friend(const A<T>&);

// Refence: https://isocpp.org/wiki/faq/templates#template-friends
template <typename T>
class A {
 public:
  friend void fn_friend<>(const A<T>&);
  friend void fn_friend_1(const A<T>& x) {
    printf("fn_friend_1 A.val: %d\n", x.val);
  }

 private:
  int val = 101;
};

template <typename T>
void fn_friend(const A<T>& x) {
  printf("fn_friend A.val: %d\n", x.val);
}

#endif  // EXTERN_TEMPLATE_H_