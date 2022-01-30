#include <iostream>
#include <memory>

int main() {
  std::shared_ptr<int> int_ptr;
  int_ptr.reset(new int(101));
  std::shared_ptr<int> int_ptr_1 = int_ptr;
  *int_ptr;
  std::weak_ptr<int> weak_int_ptr(int_ptr);
  weak_int_ptr.lock();
  std::cout << int_ptr.use_count() << std::endl;
  return 0;
}