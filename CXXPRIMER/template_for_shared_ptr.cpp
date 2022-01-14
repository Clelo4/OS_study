/**
 * @file template_for_shared_ptr.cpp
 * @author Jack
 * @mail chengjunjie.jack@gmail.com
 * @date 2022-01-13
 * @version 0.1
 *
 * @copyright Copyright (c) 2022
 */
#include <algorithm>
#include <iostream>
#include <memory>
#include <string>

using namespace std;

// shared_ptr;
template <typename T>
class My_shared_ptr {
 public:
  ~My_shared_ptr() {
    size_t left = *ref_count;
    _reduce_ref();
    if (left == 1) {
      delete ref_count;
      cout << "delete ref_count" << endl;
    }
  }
  My_shared_ptr() : ptr(nullptr), ref_count(new size_t(0), deleter(nullptr)) {}
  explicit My_shared_ptr(T *);
  template <typename Function_t>
  My_shared_ptr(T *, Function_t);
  My_shared_ptr(const My_shared_ptr &) noexcept;
  template <typename Function_t>
  My_shared_ptr(const My_shared_ptr &, Function_t) noexcept;
  My_shared_ptr &operator=(const My_shared_ptr &) noexcept;

  operator bool() const;
  T &operator*();
  T *get() { return ptr; }
  void swap(My_shared_ptr &b);

  bool unique() { return ref_count == 1; }
  size_t use_count() { return *ref_count; }

 private:
  T *ptr;
  size_t *ref_count;
  void *deleter;
  void _reduce_ref() {
    if (--*ref_count == 0) {
      if (deleter == nullptr) {
        cout << "delete ptr" << endl;
        delete ptr;
      } else {
        cout << "custom deleter start" << endl;
        // TODO 如何将deleter作为函数进行调用
        deleter(ptr);
        cout << "custom deleter end" << endl;
      }
    }
  };
};

template <typename T>
inline My_shared_ptr<T>::My_shared_ptr(T *t)
    : ptr(t), ref_count(new size_t(1)), deleter(nullptr) {}

template <typename T>
template <typename Function_t>
inline My_shared_ptr<T>::My_shared_ptr(T *t, Function_t fn)
    : ptr(t), ref_count(new size_t(1)) {
  cout << "Here" << endl;
  Function_t *delete_ptr = new Function_t(fn);
  deleter = delete_ptr;
}

template <typename T>
inline My_shared_ptr<T>::My_shared_ptr(const My_shared_ptr &other_ptr) noexcept
    : ref_count(other_ptr.ref_count),
      ptr(other_ptr.ptr),
      deleter(other_ptr.deleter) {
  ++*ref_count;
  cout << "copy constructor" << endl;
}

template <typename T>
template <typename Function_t>
inline My_shared_ptr<T>::My_shared_ptr(const My_shared_ptr &b,
                                       Function_t fn) noexcept
    : ptr(b.ptr), ref_count(b.ref_count) {
  cout << "Here" << endl;
  ++*ref_count;
  Function_t *delete_ptr = new Function_t(fn);
  deleter = delete_ptr;
}

template <typename T>
My_shared_ptr<T> &My_shared_ptr<T>::operator=(
    const My_shared_ptr &other_ptr) noexcept {
  if (other_ptr.ref_count == ref_count) return *this;
  _reduce_ref();
  ref_count = other_ptr.ref_count;
  ptr = other_ptr.ptr;
  deleter = other_ptr.deleter;
}

template <typename T>
void swap(My_shared_ptr<T> &a, My_shared_ptr<T> &b) {
  a.swap(b);
}

template <typename T>
inline My_shared_ptr<T>::operator bool() const {
  return ptr == nullptr;
}

template <typename T>
inline T &My_shared_ptr<T>::operator*() {
  return *this;
}

template <typename T>
inline void My_shared_ptr<T>::swap(My_shared_ptr &b) {
  using std::swap;
  swap(*this->ptr, b.ptr);
  swap(*this->ref_count, b.ref_count);
  swap(*this->deleter, b.deleter);
}

template <typename T>
struct IntDeleter {
  IntDeleter() = default;
  ~IntDeleter() = default;
  void operator()(T *t) {
    cout << "IntDeleter" << endl;
    delete t;
  }
};

int main() {
  My_shared_ptr<int> t1(new int(12), [](int *ptr) -> void {
    delete ptr;
    cout << "lambda deleter" << endl;
  });
  // My_shared_ptr<int> t2(new int(13));
  // My_shared_ptr<int> t3(t1);
  // My_shared_ptr<int> t4(t1);
  // My_shared_ptr<int> t5(t3, [](int *ptr) -> void { delete ptr; });
  cout << "t1: " << t1.use_count() << endl;
  // cout << "t2: " << t2.use_count() << endl;
  return 0;
}
