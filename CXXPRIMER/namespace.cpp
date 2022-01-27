/**
 * @file namespace.cpp
 * @author Jack (chengjunjie.jack@gmail.com)
 * @brief
 * @version 0.1
 * @date 2022-01-27
 *
 * @copyright Copyright (c) 2022
 */

#include <iostream>

namespace my_name {
namespace nested_name {
static int a = 12;
int b = 13;
}  // namespace nested_name
namespace std1 {}
using namespace std;
// #include <cassert> // 不能在命名空间内使用#include
}  // namespace my_name

int main() {
  using namespace std;
  using namespace my_name::nested_name;
  cout << a << " " << b << endl;
  return 0;
}
