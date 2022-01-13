/**
 * @file template_for_shared_ptr.cpp
 * @author Jack
 * @mail chengjunjie.jack@gmail.com
 * @date 2022-01-13
 * @version 0.1
 *
 * @copyright Copyright (c) 2022
 */
#include <iostream>
#include <memory>
#include <string>

using namespace std;

template <typename T>
class My_shared_ptr {
 public:
  My_shared_ptr() : ptr(nullptr), ref_count(new size_t(0), deleter(nullptr)) {}
  explicit My_shared_ptr(T *t)
      : ptr(t), ref_count(new size_t(0), deleter(nullptr)) {}
  ~My_shared_ptr() { _reduce_ref(); }

  template <typename N>
  My_shared_ptr(const My_shared_ptr<N> &);

  template <typename N>
  My_shared_ptr &operator=(const My_shared_ptr<N> &);

  operator bool() const;
  T &operator*();
  T *get() { return ptr; }
  void swap(My_shared_ptr &b);

  bool unique() { return ref_count == 1; }
  size_t use_count() { return ref_count }

 private:
  T *ptr;
  size_t *ref_count;
  void *deleter;
  void _reduce_ref() {
    if (--*ref_count == 0) {
      if (deleter == nullptr)
        delete ptr;
      else
        deleter(ptr);
    }
  };
};

template <typename T>
template <typename N>
inline My_shared_ptr<T>::My_shared_ptr(const My_shared_ptr<N> &other_ptr)
    : ref_count(other_ptr.ref_count),
      ptr(other_ptr.ptr),
      deleter(other_ptr.deleter) {
  ++*ref_count;
}

template <typename T>
template <typename N>
My_shared_ptr<T> &My_shared_ptr<T>::operator=(
    const My_shared_ptr<N> &other_ptr) {
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
  My_shared_ptr<int> t(new int(12));
  My_shared_ptr<int> t1(new int(13));
  // t1 = t;
  return 0;
}
