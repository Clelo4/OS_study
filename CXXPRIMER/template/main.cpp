#include <type_traits>

#include "extern_template.h"
#include "specializations.h"
#include "template.h"
#include "template_proxy.h"

extern template void extern_fn<int>();
extern template class A<int>;
extern template class Base_derived<int>;

int main() {
  foo("abc");
  extern_fn<int>();
  A<int> a;
  fn_friend(a);
  fn_friend_1(a);
  Base_derived<int> b;
  Base_derived_two<int> b_two;
  // b_two.g();
  Base<int> *ptr = &b_two;
  Base_derived<int> *ptr_1 = &b_two;
  ptr->g();
  ptr_1->fn();

  test_template_proxy();
  return 0;
}