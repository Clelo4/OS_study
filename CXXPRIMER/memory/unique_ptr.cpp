#include <iostream>
#include <memory>

int main() {
  std::unique_ptr<int> ptr(new int(101));
  std::unique_ptr<int> ptr_copy_1(std::move(ptr));
  std::unique_ptr<int> ptr_copy_2 = std::move(ptr_copy_1);
  std::cout << *ptr_copy_2 << std::endl;
  return 0;
}