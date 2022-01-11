/**
 * @file lambda_and_bind.cpp
 * @author Jack
 * @mail chengjunjie.jack@gmail.com
 * @date 2022-01-12
 * @version 0.1
 *
 * @copyright Copyright (c) 2022
 */

#include <algorithm>
#include <functional>
#include <iostream>
#include <vector>

int except_a0 = 0;
using namespace std;
using namespace std::placeholders;

void lambda() {
  static int except_a1 = 1;
  int except_a2 = 2;
  vector<int> numbers{0, 1, 2, 23, 324, 435, 435, 657, 67, 3345, 43};
  sort(numbers.begin(), numbers.end(),
       [](const int &a, const int &b) -> bool { return a > b; });
  for_each(numbers.begin(), numbers.end(),
           [&except_a2](const int &num) -> void {
             if (num != except_a0 && num != except_a1 && num != except_a2)
               cout << num << endl;
           });
  // mutable for catch by value
  int v1 = 42;
  auto f = [v1]() mutable -> int { return ++v1; };
  v1 = 0;
  cout << f() << endl;
}

void filter(int val, int a, int b, int c) {
  if (val != a && val != b && val != c) {
    cout << val << endl;
  }
}

void bind_fn() {
  vector<int> numbers{0, 1, 2, 23, 324, 435, 435, 657, 67, 3345, 43};
  auto f = bind(
      [](int val, int a, int b, int c) -> void {
        if (val != a && val != b && val != c) cout << val << endl;
      },
      _1, 43, 2, 0);
  for_each(numbers.begin(), numbers.end(), f);
}

int main() {
  lambda();
  bind_fn();
  return 0;
}
