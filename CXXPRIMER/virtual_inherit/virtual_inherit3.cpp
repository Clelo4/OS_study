class Base {
 public:
  Base() {}
  Base(int a) {}
  Base(const Base&) {}
};

class A : public Base {
 public:
  A() {}
  A(int a) : Base(a) {}
  A(const A& a) : Base(a) {}
};

int main() {
  A a;
  return 0;
}