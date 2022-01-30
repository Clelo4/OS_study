#include <iostream>

class A {
 public:
  virtual void fn() {
    using namespace std;
    cout << "A::fn()" << endl;
  }
  virtual void fn1() {
    using namespace std;
    cout << "A::fn1()" << endl;
  }
};

class B : public A {
 public:
  using A::fn;
  void fn(int a) {
    using namespace std;
    cout << "A::fn()" << endl;
  }
  void fn1(int a) {
    using namespace std;
    cout << "A::fn1()" << endl;
  }
};

int main() {
  B b;
  b.fn(12);
  b.fn();
  b.fn1(13);
  // b.fn1(); // B中的fn1隐藏A的fn1
  return 0;
}