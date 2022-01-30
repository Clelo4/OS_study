/**
 * @file nested_class.cpp
 * @author your name (you@domain.com)
 * @brief
 * @version 0.1
 * @date 2022-01-24
 *
 * @copyright Copyright (c) 2022
 *
 */

#include <cstdio>
#include <iostream>

class Wrapper {
 public:
  class Nest {
   public:
    Nest(){};
    Nest(const Nest&);
    static int i;
  };

  // Nest::Nest(const Nest&) {} // error

 private:
};
int Wrapper::Nest::i = 12;
Wrapper::Nest::Nest(const Nest&) {}

int main() {
  using namespace std;
  cout << Wrapper::Nest::i << endl;
  return 0;
}