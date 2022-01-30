#include <iostream>
#include <memory>

int main() {
  // make_shared<T[]> not supported
  // std::shared_ptr<int[]> int_ptr(std::make_shared<int[]>());
  std::unique_ptr<int[]> int_uni_ptr(new int[101]);
  auto mem1 = &decltype(int_uni_ptr)::get;
  std::cout << (int_uni_ptr.*mem1)() << std::endl;
  std::cout << int_uni_ptr.get() << std::endl;
  return 0;
}
