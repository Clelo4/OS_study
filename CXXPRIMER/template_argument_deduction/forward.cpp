/**
 * @file forward.cpp
 * @author Jack
 * @mail chengjunjie.jack@gmail.com
 * @date 2022-01-14
 * @version 0.1
 *
 * @copyright Copyright (c) 2022
 */

#include <iostream>
#include <utility>

using namespace std;

template <typename T>
void fn(T&& arg) {
  cout << arg << endl;
}

template <typename T>
void wrapper(T&& arg) {
  fn(std::forward<T>(arg));
}

int main() {
  int i = 12, *pi = &i, &li = i;
  int&& ri = 13;
  wrapper(i);
  wrapper(pi);
  wrapper(li);
  wrapper(ri);
  wrapper(std::move(ri));
  wrapper(12);
  fn(12);
  return 0;
}
