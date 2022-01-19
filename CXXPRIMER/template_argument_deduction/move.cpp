/**
 * @file move.cpp
 * @author Jack
 * @mail chengjunjie.jack@gmail.com
 * @date 2022-01-14
 * @version 0.1
 *
 * @copyright Copyright (c) 2022
 */

#include <utility>

int main() {
  int i = 1, &li = i;
  int&& ri = 2;

  // rvalue reference to type 'int' cannot bind to lvalue of type 'int'
  // int&& move_ri = ri;

  int&& move_ri_2 = std::move(i);
  int&& move_ri_3 = std::move(li);
  int&& move_ri_4 = std::move(ri);
  return 0;
}
