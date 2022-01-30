#include <algorithm>
#include <functional>
#include <iostream>
#include <string>

template <typename T>
std::ostream& print(std::ostream& os, const T& a, const T& b) {
  return os << a << b << std::endl;
}

int main() {
  using namespace std::placeholders;
  print(std::cout, 1, 1);
  auto bindWrapper = std::bind(print<int>, std::ref(std::cout), 101, _1);
  bindWrapper(102);
  return 0;
}