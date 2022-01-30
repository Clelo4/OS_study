#include <iostream>
#include <memory>
#include <string>

int main() {
  constexpr int n = 2;
  std::allocator<std::string> alloc;
  auto const p = alloc.allocate(n);
  auto use_p = p;
  std::string s;
  int i = 0;
  while (std::cin >> s && i < n) {
    alloc.construct(use_p++, s);
    ++i;
  }
  for (int j = 0; j < i; ++j) {
    alloc.destroy(p + j);
  }
  alloc.deallocate(p, n);

  return 0;
}