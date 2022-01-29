#include <iostream>

class Class {};
class Base : public Class {};
class D1 : virtual public Base {};
class D2 : virtual public Base {};
class MI : public D1, public D2 {};
class Final : public MI, public Class {};

Base *pb;
Class *pc;
MI *pmi;
D2 *pd2;

int main() {
  // pb = new Class;
  // pc = new Final;
  // pmi = pb;
  pd2 = pmi;
  return 0;
}
