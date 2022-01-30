#include <iostream>

class Base {
 public:
  Base() { std::cout << "Base()" << std::endl; }
  Base(int a) { std::cout << "Base(int)" << std::endl; }
  void print_ptr() { std::cout << this << std::endl; }
  ~Base() { std::cout << "~Base()" << std::endl; }
};
class A : virtual public Base {
 public:
  A() { std::cout << "A()" << std::endl; };
  ~A() { std::cout << "~A()" << std::endl; }
};
class B : virtual public Base {
 public:
  B() { std::cout << "B()" << std::endl; };
  ~B() { std::cout << "~B()" << std::endl; }
};
class C : virtual public A, virtual public B {
 public:
  C(){};
};

class D : public C {
 public:
  D(int a) : Base(a) {}
};

int main() {
  D c(12);
  c.print_ptr();
  return 0;
}